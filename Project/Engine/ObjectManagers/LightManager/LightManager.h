#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/LightStruct.h"
#include "Math/Utility/MathUtility.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;

/// <summary>
/// ライト管理クラス
/// </summary>
class LightManager {
public:
	LightManager(DXGI* dxgi, DirectXCommand* directXCommand);
	~LightManager();

	void Update();

	void TransferDirectionalLight(uint32_t paramIndex);
	void SetDirectionalLight(const DirectionalLight& directionalLight);

	void TransferDirectionalLightCamera(uint32_t paramIndex);

private:
	void CreateDirectionalLightResource();
	void MapDirectionalLightData();

	void CreateDirectionalLightCameraResource();
	void MapDirectionalLightCameraData();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSrvUavManager(SRVUAVManager* srvuavManager);

private:
	//========================
	// DirectionalLightCamera
	//========================
	ComPtr<ID3D12Resource> directionalLightCameraResource_;
	DirectionalLightCameraForGPU* directionalLightCameraData_ = nullptr;

	// 水平視野角(度数法)
	float fovYDegrees_ = 60.0f;
	// 水平方向視野角
	float fovY_ = fovYDegrees_ * (std::numbers::pi_v<float> / 180.0f);
	// アスペクト比
	const float aspectRaito_ = 1.0f;
	// ニアクリップ距離
	const float nearClipRange_ = 0.1f;
	// ファークリップ距離
	const float farClipRange_ = 500.0f;

	//========================
	// DirectionalLight
	//========================
	DirectionalLight directionalLight_{};
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLightForGPU* directionalLightData_ = nullptr;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvuavManager_ = nullptr;
};