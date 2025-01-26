#pragma once

#include "3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

class Sphere: public BasePrimitiveShape3D {
public:
	Sphere(const std::string& textureFilePath = "");
	~Sphere()override = default;

	void CreateShape()override;
	void Update()override;
private:
	// 球分割数
	const uint32_t kSubdivision = 16;
};