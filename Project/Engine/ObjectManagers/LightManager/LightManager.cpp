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
	directionalLight_.direction = Normalize(directionalLight.direction);
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

void LightManager::CreateDirectionalLightCameraResource() {
	directionalLightCameraResource_ = dxgi_->CreateBufferResource(sizeof(DirectionalLightCameraForGPU));
}

void LightManager::MapDirectionalLightCameraData() {
	diractionalLightCameraData_ = nullptr;
	directionalLightCameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&diractionalLightCameraData_));
	diractionalLightCameraData_->viewProjection = MakeIdentityMatrix4x4();
}

Matrix4x4 LightManager::MakeLightViewProjectionMatrix(const Vector3& lightDirection, const Vector3& target, float width, float height, float nearClip, float farClip) {
	Vector3 dir = Normalize(lightDirection);
	Vector3 pos = target - dir * 10.0f; // ライト位置（ターゲットから10m上空）

	// カメラの軸を構築
	Vector3 zAxis = Normalize(target - pos);
	Vector3 xAxis = Normalize(Cross(MakeUpVector3(), zAxis));
	Vector3 yAxis = Cross(zAxis, xAxis);

	// ビュー行列の作成
	Matrix4x4 view = {
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		-Dot(xAxis, pos), -Dot(yAxis, pos), -Dot(zAxis, pos), 1.0f
	};

	// 正射影行列（シャドウマップ用）
	float left = -width / 2.0f;
	float right = width / 2.0f;
	float bottom = -height / 2.0f;
	float top = height / 2.0f;
	Matrix4x4 proj = MakeOrthographicMatrix(left, top, right, bottom, nearClip, farClip);

	return view * proj;
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
