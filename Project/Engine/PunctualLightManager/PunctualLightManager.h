#pragma once

// C++
#include <map>
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/PunctualLightStruct.h"
#include "Enums/PunctualLightDataEnum.h"

class DXGI;
class SRVUAVManager;

class PunctualLightManager {
public:
	PunctualLightManager(DXGI* dxgi, SRVUAVManager* srvuavManager);
	~PunctualLightManager();

	void Initialize(DXGI* dxgi, SRVUAVManager* srvuavManager);
	void Update();
	void TransferLightsData();

	void AddNewLight(const std::string& lightName, const PunctualLightData& lightData);

	void SetLightData(const std::string& lightName, const PunctualLightData& lightData);
private:
	void CreateLightsResource();
	void MapLightsData();

	void CreateLightCountResource();
	void MapLightCountData();

	void UpdateLightsData();
	void UpdateLightCountData();
private:
	void SetDXGI(DXGI* dxgi);
	void SetSrvUavManager(SRVUAVManager* srvuavManager);
private:
	// ライトの最大数
	const uint32_t kMaxLightNum_ = 4;

	// ライト
	std::map<std::string, PunctualLightData> light_;

	// ライトのリソース(instancing用)
	ComPtr<ID3D12Resource> lightsResource_ = nullptr;
	// GPUに送るライトのデータ
	PunctualLightData* lightsData_ = nullptr;
	// SrvIndex
	uint32_t srvIndex_ = 0;

	// ライトの数を送るリソース
	ComPtr<ID3D12Resource> lightCountResource_ = nullptr;
	// GPUに送るライトの数のデータ
	PunctualLightCount* lightCountData_ = nullptr;
	// 現在のライト数
	uint32_t currentLightNum_ = 0;

private:
	DXGI* dxgi_ = nullptr;
	SRVUAVManager* srvuavManager_ = nullptr;
};