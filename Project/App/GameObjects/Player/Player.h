#pragma once

#include "3D/GameObject3D/GameObject3D.h"

class Player :public GameObject3D {
public:
	Player(const std::string& objectName, const EulerTransform3D& transform = EulerTransform3D{});
	~Player()override = default;

	void Initialize()override;

	void Update()override;

private:

};