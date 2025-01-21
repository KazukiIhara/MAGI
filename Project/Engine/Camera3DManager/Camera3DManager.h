#pragma once

#include <memory>

#include "3D/Camera3D/Camera3D.h"

/// <summary>
/// 3D描画用カメラのマネージャ
/// </summary>
class Camera3DManager {
public:
	Camera3DManager();
	~Camera3DManager();

	void Initialize();
	void Update();
	void TransferCamera();

private:
	// デバッグカメラ切り替え変数
	bool isDebugCamera_ = false;
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_;
};