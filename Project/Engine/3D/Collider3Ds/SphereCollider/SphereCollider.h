#pragma once

#include "3D/Collider3Ds/BaseCollider3D/BaseCollider3D.h"

class SphereCollider:public BaseCollider3D {
public:
	SphereCollider(GameObject3D* owner, Collider3DType type);
	~SphereCollider()override = default;

	void Draw()override;

private:
	float radius_ = 1.0f;
};