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
	UpdateLightsData();
	UpdateLightCountData();
}

void PunctualLightManager::TransferLightsData() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();


}

void PunctualLightManager::AddNewLight(const std::string& lightName, const PunctualLightData& lightData) {
	// 1. ライト名の重複チェック
	auto it = light_.find(lightName);
	assert(it == light_.end() && "ライト名が既に存在します。");

	// 2. 最大ライト数のチェック
	assert(light_.size() < kMaxLightNum_ && "最大ライト数に達しています。これ以上ライトを追加できません。");

	// 3. ライトの追加
	light_.emplace(lightName, lightData);
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

void PunctualLightManager::UpdateLightsData() {
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

void PunctualLightManager::UpdateLightCountData() {
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
