#pragma once

#include <string>
#include "Math/Utility/MathUtility.h"

class WorldEntity {
public:
	virtual void Initialize();
public:
	// 名前
	std::string name_;
	// ワールド座標
	Vector3 worldPosition_{};
};