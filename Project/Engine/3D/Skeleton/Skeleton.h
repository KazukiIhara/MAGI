#pragma once

// C++
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Structs/ModelStruct.h"

#include "3D/RenderObjects3D/Primitive3D/Primitive3D.h"

class Skeleton {
public:
	Skeleton(const Node& rootNode);
	~Skeleton();

	void Update();

	//
	// ForDebug
	//

	void Draw();

private:
	void Initialize(const Node& rootNode);
	// ジョイント作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);
public:
	int32_t root;
	std::unordered_map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;

	//
	// ForDebug
	//

	std::vector<std::unique_ptr<Primitive3D>> spheres_;

};