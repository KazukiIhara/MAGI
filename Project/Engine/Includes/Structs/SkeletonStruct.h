#pragma once

// C++
#include <unordered_map>
#include <optional>

// MyHedder
#include "Structs/ObjectStruct.h"

// ノード
struct Node {
	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

// ジョイント
struct Joint {
	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeletonSpaceMatrix;
	std::string name;
	std::vector<uint32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

// スケルトン
struct SkeletonData {
	int32_t root;
	std::unordered_map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};