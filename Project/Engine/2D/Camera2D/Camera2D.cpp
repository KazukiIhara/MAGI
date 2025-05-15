#include "Camera2D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Camera2D::Camera2D() {
	Initialize();
}

void Camera2D::Initialize() {
	CreateCameraResource();
	MapCameraData();
}

void Camera2D::Update() {
	// カメラ特有の更新処理があれば

}

void Camera2D::UpdateData() {

}

void Camera2D::TransferCamera(uint32_t rootParameterIndex) {
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, cameraResource_->GetGPUVirtualAddress());
}

void Camera2D::CreateCameraResource() {
	cameraResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Camera2DForGPU));
}

void Camera2D::MapCameraData() {
	cameraData_ = nullptr;
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->viewProjection = MakeIdentityMatrix4x4();
}