#pragma once

#include <string>
#include "Math/Utility/MathUtility.h"

class WorldEntity {
public:
	WorldEntity();
	virtual ~WorldEntity() = default;
protected:
	std::string& GetName();

public:
	// 名前
	std::string name_;
	// ワールド座標
	Vector3 worldPosition_{};
};