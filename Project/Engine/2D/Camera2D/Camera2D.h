#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/CameraStruct.h"

/// <summary>
/// 2D用カメラ
/// </summary>
class Camera2D {
public:
	Camera2D();
	virtual ~Camera2D() = default;

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();
	// データ更新
	virtual void UpdateData();
	// 定数バッファに転送
	void TransferCamera(uint32_t rootParameterIndex);

private:
	// カメラのリソースを作成
	void CreateCameraResource();
	// カメラのデータをマップ
	void MapCameraData();

private:
	// Camera用リソース
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	// Camera用データ
	Camera2DForGPU* cameraData_ = nullptr;
};