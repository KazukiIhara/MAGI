#pragma once

#include <map>
#include <string>
#include <memory>

/// <summary>
/// 2D描画用のカメラマネージャ
/// </summary>
class Camera2DManager {
public:
	Camera2DManager();
	~Camera2DManager();

	void Initialize();
	void Update();
	void TransferCurrentCamera(uint32_t rootParameterIndex);

private:

};