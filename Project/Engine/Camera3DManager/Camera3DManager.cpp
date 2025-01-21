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
}

void Camera3DManager::Update() {
	sceneCamera_->Update();
}

void Camera3DManager::TransferCamera() {
	sceneCamera_->TransferCamera();
}