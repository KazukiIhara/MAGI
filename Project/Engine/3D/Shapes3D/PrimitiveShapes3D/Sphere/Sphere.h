#pragma once

#include "3D/Shapes3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

class Sphere: public BasePrimitiveShape3D {
public:
	Sphere(const std::string& textureFilePath = "");
	~Sphere()override = default;
	void SetShape()override;
	void Update()override;
private:
};