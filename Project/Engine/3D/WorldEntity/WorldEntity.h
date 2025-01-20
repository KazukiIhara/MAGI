#pragma once

#include <string>
#include "Math/Utility/MathUtility.h"

class WorldEntity {
public:
	virtual void Initialize();
protected:
	Vector3 worldPosition_{};
	std::string name_;
};