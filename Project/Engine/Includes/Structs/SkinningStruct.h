#pragma once

// C++
#include <cstdint>
#include <array>
#include <span>

// DirectX
#include <d3d12.h>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "DirectX/ComPtr/ComPtr.h"

// 影響を受ける頂点の数
const uint32_t kNumMaxInfluence = 4;
// 影響を受ける頂点
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};

struct SkinningInformationForGPU {
	uint32_t numVertices;
};

struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};