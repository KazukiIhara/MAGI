#include "LightManager.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

LightManager::LightManager(DXGI* dxgi, DirectXCommand* directXCommand) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);

	CreateDirectionalLightResource();
	MapDirectionalLightData();

	CreateDirectionalLightCameraResource();
	MapDirectionalLightCameraData();

	Logger::Log("LightManager Initialize\n");
}

LightManager::~LightManager() {
	Logger::Log("LightManager Finalize\n");
}

void LightManager::Update() {
	// ライト情報の更新
	directionalLightData_->direction = directionalLight_.direction;
	directionalLightData_->intensity = directionalLight_.intensity;
	directionalLightData_->color = directionalLight_.color;


	Matrix4x4 translateMat = MakeTranslateMatrix(-directionalLight_.direction * 5.0f);
	Matrix4x4 rotateMat = MakeRotateMatrix(DirectionToQuaternion(directionalLight_.direction));

	Matrix4x4 viewMat = Inverse(rotateMat * translateMat);
	Matrix4x4 projMat = MakePerspectiveFovMatrix(fovY_, aspectRaito_, nearClipRange_, farClipRange_);

	diractionalLightCameraData_->viewProjection = viewMat * projMat;
}

void LightManager::TransferDirectionalLight(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightResource_->GetGPUVirtualAddress());
}

void LightManager::SetDirectionalLight(const DirectionalLight& directionalLight) {
	directionalLight_.direction = Normalize(directionalLight.direction);
	directionalLight_.intensity = directionalLight.intensity;
	directionalLight_.color = directionalLight.color;
}

void LightManager::TransferDirectionalLightCamera(uint32_t paramIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// ライト情報を送る
	commandList->SetGraphicsRootConstantBufferView(paramIndex, directionalLightCameraResource_->GetGPUVirtualAddress());
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

void LightManager::CreateDirectionalLightCameraResource() {
	directionalLightCameraResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightCameraForGPU));
}

void LightManager::MapDirectionalLightCameraData() {
	diractionalLightCameraData_ = nullptr;
	directionalLightCameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&diractionalLightCameraData_));
	diractionalLightCameraData_->viewProjection = MakeIdentityMatrix4x4();
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
