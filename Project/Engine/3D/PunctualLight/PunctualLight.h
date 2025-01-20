#pragma once

// C++
#include <map>
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/PunctualLightStruct.h"
#include <Enums/PunctualLightDataEnum.h>

class PunctualLightManager {
public:
	PunctualLightManager();
	~PunctualLightManager();

	void Initialize();
	void Update();
	void TransferLightsData();

	void AddNewLight(const std::string& lightName, const PunctualLightData& lightData);

	void SetLightData(const std::string& lightName, const PunctualLightData& lightData);
private:
	void CreateLightsResource();
	void MapLightsData();
	
	void CreateLightCountResource();
	void MapLightCountData();
private:
	// ライトの最大数
	const uint32_t kMaxLightNum_ = 4;

	// ライトのデータ
	std::map<std::string, PunctualLightData> lightData_;

	// ライトのリソース(instancing用)
	ComPtr<ID3D12Resource> lightsResource_ = nullptr;
	// GPUに送るライトのデータ
	PunctualLightData* lightsData_ = nullptr;

	// ライトの数を送るリソース
	ComPtr<ID3D12Resource> lightCountResource_ = nullptr;
	// GPUに送るライトの数のデータ
	PunctualLightCount* lightCountData_ = nullptr;
	// 現在のライト数
	uint32_t currentLightNum_ = 0;
};