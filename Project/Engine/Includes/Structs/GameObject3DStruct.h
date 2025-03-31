#pragma once

#include "Math/Utility/MathUtility.h"
#include "Structs/ObjectStruct.h"

struct GameObject3DData {
	EulerTransform3D transform;
	Vector4 color;
};

struct GameObject3DDataForGPU {
	Matrix4x4 worldMatrix;
	Vector4 color;
};