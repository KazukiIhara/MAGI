#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"

// シェーダーに送るカメラ座標
struct CameraForGPU {
	Vector3 worldPosition;
	Matrix4x4 viewProjection;
};

struct ViewProjectionForGPU {
	Matrix4x4 viewProjection;
};