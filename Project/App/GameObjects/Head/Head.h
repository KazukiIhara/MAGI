#pragma once

#include "3D/GameObject3D/GameObject3D.h"

class Head :public GameObject3D {
public:
	Head(const std::string& objectName, const EulerTransform3D& transform = EulerTransform3D{});
	~Head()override = default;

	void Initialize()override;

	void Update()override;

private:

};