#pragma once

#include "Structs/ModelStruct.h"

class Skeleton {
public:
	Skeleton(const Node& rootNode);
	~Skeleton();

	void Update();
private:
	void Initialize(const Node& rootNode);
	// ジョイント作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
private:
	// スケルトン
	SkeletonData skeleton_;
};