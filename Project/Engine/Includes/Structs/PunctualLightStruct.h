#pragma once

#include <cstdint>
#include "Math/Types/Vector3.h"
#include "Math/Types/Vector4.h"

/// <summary>
/// ライトのデータ
/// </summary>
struct PunctualLightData {
	// 全ライト共通
	uint32_t type;
	Vector4 color;
	float intensity;

	// SpotPoint共通
	Vector3 position;

	// DirectionalLight用
	Vector3 direction;

	// PointLight用
	float radius;
	float decay;

	// SpotLight用
	Vector3 spotDirection;
	float cosAngle;
	float cosFalloffStart;
};

/// <summary>
/// GPUに送るライトの数
/// </summary>
struct PunctualLightCount {
	uint32_t num;
};