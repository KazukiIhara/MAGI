#include "Camera3D.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

Camera3D::Camera3D() {
	Initialize();
}

Camera3D::~Camera3D() {

}

void Camera3D::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.rotate_ = kDefaultCameraRotate_;
	worldTransform_.translate_ = kDefaultCameraTranslate_;
	worldTransform_.Update();

	Matrix4x4 viewMatrix = Inverse(worldTransform_.worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRaito_, nearClipRange_, farClipRange_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	backFrontMatrix_ = MakeRotateYMatrix(std::numbers::pi_v<float>);

	CreateCameraResource();
	MapCameraData();
}

void Camera3D::Update() {
	worldTransform_.Update();
	Matrix4x4 viewMatrix = Inverse(worldTransform_.worldMatrix_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	billboardMatrix_ = backFrontMatrix_ * worldTransform_.worldMatrix_;
	// 平行移動成分を削除
	billboardMatrix_.m[3][0] = 0.0f;
	billboardMatrix_.m[3][1] = 0.0f;
	billboardMatrix_.m[3][2] = 0.0f;

	UpdateCameraData();
}

void Camera3D::TransferCamera() {
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(2, cameraResource_->GetGPUVirtualAddress());
}

Matrix4x4 Camera3D::GetViewProjectionMatrix() const {
	return viewProjectionMatrix_;
}

void Camera3D::CreateCameraResource() {
	cameraResource_ = MAGISYSTEM::CreateBufferResource(sizeof(CameraForGPU));
}

void Camera3D::MapCameraData() {
	cameraData_ = nullptr;
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	UpdateCameraData();
}

void Camera3D::UpdateCameraData() {
	cameraData_->worldPosition = worldTransform_.worldPosition_;
	cameraData_->viewProjection = viewProjectionMatrix_;
}
