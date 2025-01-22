#pragma once

#include <string>
#include "Math/Utility/MathUtility.h"

class WorldEntity {
public:
	WorldEntity();
	virtual ~WorldEntity() = default;

	std::string& GetName();
	Vector3& GetWorldPosition();
public:
	// 名前
	std::string name_;
	// ワールド座標
	Vector3 worldPosition_{};
};