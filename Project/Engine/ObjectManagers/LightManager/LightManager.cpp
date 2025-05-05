#include "LightManager.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

LightManager::LightManager(DXGI* dxgi, DirectXCommand* directXCommand) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	CreateDirectionalLightResource();
	MapDirectionalLightData();

	Logger::Log("LightManager Initialize\n");
}

LightManager::~LightManager() {
	Logger::Log("LightManager Finalize\n");
}

void LightManager::Update() {
	directionalLightData_->direction = directionalLight_.direction;
	directionalLightData_->intensity = directionalLight_.intensity;
	directionalLightData_->color = directionalLight_.color;
}

void LightManager::TransferDirectionalLight(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightResource_->GetGPUVirtualAddress());
}

void LightManager::SetDirectionalLight(const DirectionalLight& directionalLight) {
	directionalLight_.direction = directionalLight.direction;
	directionalLight_.intensity = directionalLight.intensity;
	directionalLight_.color = directionalLight.color;
}

void LightManager::CreateDirectionalLightResource() {
	directionalLightResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightForGPU));
}

void LightManager::MapDirectionalLightData() {
	directionalLightData_ = nullptr;
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
	directionalLightData_->color = { 1.0f,1.0f,1.0f };
}

void LightManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void LightManager::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void LightManager::SetSrvUavManager(SRVUAVManager* srvuavManager) {
	assert(srvuavManager);
	srvuavManager_ = srvuavManager;
}
