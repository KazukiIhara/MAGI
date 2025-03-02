#include "Camera3DManager.h"

#include "Logger/Logger.h"
#include <cassert>

Camera3DManager::Camera3DManager() {
	Initialize();
	Logger::Log("Initialize CameraManager\n");
}

Camera3DManager::~Camera3DManager() {
	Logger::Log("Finalize CameraManager\n");
}

void Camera3DManager::Initialize() {
#ifdef _DEBUG
	debugCamera_ = std::make_unique<DebugCamera3D>("DebugCamera");
#endif // _DEBUG
}

void Camera3DManager::Update() {
#ifdef _DEBUG
	if (isDebugCamera_) {
		debugCamera_->UpdateData();
		return;
	}
#endif // _DEBUG

}

void Camera3DManager::TransferCurrentCamera(uint32_t rootParameterIndex) {
#ifdef _DEBUG
	// デバッグカメラフラグがオンの場合デバッグカメラを転送
	if (isDebugCamera_) {
		debugCamera_->TransferCamera(rootParameterIndex);
		return;
	}
#endif // _DEBUG
	// 現在選択中のカメラを見つけて転送
	Find(currentCameraName_)->TransferCamera(rootParameterIndex);
}

std::string Camera3DManager::Add(std::unique_ptr<Camera3D> newCamera3D) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = newCamera3D->name;
	int suffix = 1;

	// 同じ名前が既に存在する場合、ユニークな名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(cameras3D_.begin(), cameras3D_.end(), [&](const auto& camera3D) {
			return camera3D.second->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = newCamera3D->name + "_" + std::to_string(suffix);
		suffix++;
	}

	// ユニークな名前に変更
	newCamera3D->name = uniqueName;

	// コンテナに登録
	cameras3D_.insert(std::pair(uniqueName, std::move(newCamera3D)));

	// ユニークな名前を返す
	return uniqueName;
}

void Camera3DManager::Remove(const std::string& cameraName) {
	// 指定した名前のカメラを検索
	if (cameras3D_.contains(cameraName)) {
		// 見つかったら消す
		cameras3D_.erase(cameraName);
	} else {
		// 指定した名前のカメラがない場合止める
		assert(false && "Camera3D Not Found");
	}
}

Camera3D* Camera3DManager::Find(const std::string& cameraName) {
	// 指定した名前のオブジェクトを検索
	auto it = cameras3D_.find(cameraName);

	// 見つかった場合はポインタを返す
	if (it != cameras3D_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}

void Camera3DManager::SetCurrentCameraName(const std::string& cameraName) {
	currentCameraName_ = cameraName;
}

bool& Camera3DManager::GetIsDebugCamera() {
	return isDebugCamera_;
}
