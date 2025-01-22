#pragma once

#include "3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

class Plane: public BasePrimitiveShape3D {
public:
	Plane(const std::string& textureFilePath = "");
	virtual ~Plane() = default;

	void CreateShape()override;
	void Update()override;

private:
	float paramater0_ = 0.0f;
	float paramater1_ = 0.0f;
};