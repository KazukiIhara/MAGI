#pragma once

#include <map>
#include <string>
#include <memory>

#include "3D/Cameras3D/DebugCamera3D/DebugCamera3D.h"

/// <summary>
/// 3D描画用カメラのマネージャ
/// </summary>
class Camera3DManager {
public:
	Camera3DManager();
	~Camera3DManager();

	void Initialize();
	void Update();
	void TransferCurrentCamera(uint32_t rootParameterIndex);

	std::string Add(std::unique_ptr<Camera3D> newCamera3D);
	void Remove(const std::string& cameraName);
	Camera3D* Find(const std::string& cameraName);

	// 現在使用するカメラの名前をセット
	void SetCurrentCameraName(const std::string& cameraName);

	// デバッグカメラフラグの参照
	bool& GetIsDebugCamera();

private:
	// デバッグカメラ切り替え変数
	bool isDebugCamera_ = false;
	// 使用するカメラの名前
	std::string currentCameraName_ = "";
	// カメラコンテナ
	std::map<std::string, std::unique_ptr<Camera3D>> cameras3D_;
	// デバッグカメラ
	std::unique_ptr<DebugCamera3D> debugCamera_;
};