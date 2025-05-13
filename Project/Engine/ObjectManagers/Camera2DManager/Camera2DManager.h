#pragma once

#include <map>
#include <string>
#include <memory>

/// <summary>
/// 2D描画用のカメラマネージャ
/// </summary>
class Camera2Dmanager {
public:
	Camera2Dmanager();
	~Camera2Dmanager();

	void Initialize();
	void Update();
	void TransferCurrentCamera(uint32_t rootParameterIndex);

private:

};