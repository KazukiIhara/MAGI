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
private:

	void CreateDirectionalLightResource();
	void MapDirectionalLightData();

	Matrix4x4 MakeLightViewProjectionMatrix(const Vector3& lightDirection, const Vector3& target, float width, float height, float nearClip, float farClip);
private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSrvUavManager(SRVUAVManager* srvuavManager);

private:

	ComPtr<ID3D12Resource> lightCameraResource_;
	LightCameraForGPU* lightCameraData_ = nullptr;

	DirectionalLight directionalLight_{};
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLightForGPU* directionalLightData_ = nullptr;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvuavManager_ = nullptr;
};