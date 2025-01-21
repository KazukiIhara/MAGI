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

}

void Camera3D::Update() {

}

void Camera3D::TransferCamera() {

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
