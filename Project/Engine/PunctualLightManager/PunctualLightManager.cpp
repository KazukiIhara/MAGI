#include "PunctualLightManager.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"


PunctualLightManager::PunctualLightManager(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvuavManager) {
	Initialize(dxgi, directXCommand, srvuavManager);
	Logger::Log("PunctualLightManager Initialize\n");
}

PunctualLightManager::~PunctualLightManager() {
	Logger::Log("PunctualLightManager Finalize\n");
}

void PunctualLightManager::Initialize(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvuavManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSrvUavManager(srvuavManager);

	CreateLightsResource();
	MapLightsData();

	CreateLightCountResource();
	MapLightCountData();

	// Srvの作成
	srvIndex_ = srvuavManager_->Allocate();
	srvuavManager_->CreateSrvStructuredBuffer(srvIndex_, lightsResource_.Get(), kMaxLightNum_, sizeof(PunctualLightData));
}

void PunctualLightManager::Update() {
	// ライトの数が最大数を超えていないことを確認
	assert(light_.size() <= kMaxLightNum_);

	// 現在のライト数を設定
	currentLightNum_ = static_cast<uint32_t>(light_.size());
	lightCountData_->num = currentLightNum_;

	uint32_t index = 0;
	// ライトマップをループしてデータを更新
	for (const auto& [lightName, lightData] : light_) {
		if (index >= kMaxLightNum_) {
			// 必要に応じてエラーハンドリングを追加
			assert(false && "ライトの最大数を超えています。");
			break;
		}
		lightsData_[index] = lightData;
		index++;
	}

	// 残りのライトデータをデフォルト値で埋める
	for (; index < kMaxLightNum_; ++index) {
		PunctualLightData defaultLight{};
		lightsData_[index] = defaultLight;
	}
}

void PunctualLightManager::TransferLightsData() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライトの総数を送る
	commandList->SetGraphicsRootConstantBufferView(5, lightCountResource_->GetGPUVirtualAddress());
	// ライトのSrvStructuredBufferを送る
	commandList->SetGraphicsRootDescriptorTable(6, srvuavManager_->GetDescriptorHandleGPU(srvIndex_));
}

void PunctualLightManager::AddNewLight(const std::string& lightName, const PunctualLightData& lightData) {
	// ライト名の重複チェック
	auto it = light_.find(lightName);
	assert(it == light_.end() && "ライト名が既に存在します。");

	// 最大ライト数のチェック
	assert(light_.size() < kMaxLightNum_ && "最大ライト数に達しています。これ以上ライトを追加できません。");

	// ライトの追加
	light_.emplace(lightName, lightData);
}

void PunctualLightManager::RemoveLight(const std::string& lightName) {
	// ライト名でライトを検索
	auto it = light_.find(lightName);
	if (it != light_.end()) {
		light_.erase(it);
	} else {
		assert(false && "ライト名が存在しません。");
	}
}

void PunctualLightManager::SetLightData(const std::string& lightName, const PunctualLightData& lightData) {
	// ライト名でライトを検索
	auto it = light_.find(lightName);
	if (it != light_.end()) {
		it->second = lightData;
	} else {
		assert(false && "ライト名が存在しません。");
	}
}

void PunctualLightManager::CreateLightsResource() {
	lightsResource_ = dxgi_->CreateBufferResource(sizeof(PunctualLightData) * kMaxLightNum_);
}

void PunctualLightManager::MapLightsData() {
	lightsData_ = nullptr;
	lightsResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightsData_));

	for (uint32_t i = 0; i < kMaxLightNum_; i++) {
		PunctualLightData lightDefault{};
		lightsData_[i] = lightDefault;
	}
}

void PunctualLightManager::CreateLightCountResource() {
	lightCountResource_ = dxgi_->CreateBufferResource(sizeof(PunctualLightCount));
}

void PunctualLightManager::MapLightCountData() {
	lightCountData_ = nullptr;
	lightCountResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightCountData_));
	lightCountData_->num = currentLightNum_;
}

void PunctualLightManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void PunctualLightManager::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void PunctualLightManager::SetSrvUavManager(SRVUAVManager* srvuavManager) {
	assert(srvuavManager);
	srvuavManager_ = srvuavManager;
}
