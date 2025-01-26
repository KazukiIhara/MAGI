#pragma once

// C++
#include <string>
#include <unordered_map>

#include "Structs/ModelStruct.h"

class Skeleton {
public:
	Skeleton(const Node& rootNode, const std::unordered_map<std::string, Matrix4x4>& ibpMap);
	~Skeleton();

	void Update();

private:
	void Initialize(const Node& rootNode, const std::unordered_map<std::string, Matrix4x4>& ibpMap);
	// ジョイント作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);
public:
	int32_t root;
	std::unordered_map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};