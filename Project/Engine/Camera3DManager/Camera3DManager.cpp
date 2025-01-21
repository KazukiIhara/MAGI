#include "Camera3DManager.h"

#include "Logger/Logger.h"

Camera3DManager::Camera3DManager() {
	Initialize();
	Logger::Log("Initialize CameraManager\n");
}

Camera3DManager::~Camera3DManager() {
	Logger::Log("Finalize CameraManager\n");
}

void Camera3DManager::Initialize() {
	sceneCamera_ = std::make_unique<Camera3D>();
#ifdef _DEBUG
	debugCamera_ = std::make_unique<DebugCamera3D>();
#endif // _DEBUG
}

void Camera3DManager::Update() {
#ifdef _DEBUG
	if (isDebugCamera_) {
		debugCamera_->Update();
		return;
	}
#endif // _DEBUG

	sceneCamera_->Update();
}

void Camera3DManager::TransferCamera() {
#ifdef _DEBUG
	if (isDebugCamera_) {
		debugCamera_->TransferCamera();
		return;
	}
#endif // _DEBUG
	sceneCamera_->TransferCamera();
}

bool& Camera3DManager::GetIsDebugCamera() {
	return isDebugCamera_;
}
