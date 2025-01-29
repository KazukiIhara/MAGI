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

// 頂点が影響を受けるボーンの数
const uint32_t kNumMaxInfluence = 4;
// 頂点が影響を受けるボーンのスロット
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
	// 重み
	float weight;
	// どのメッシュの頂点か
	uint32_t meshIndex;
	// メッシュ内でのローカル頂点インデックス
	uint32_t localVertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};