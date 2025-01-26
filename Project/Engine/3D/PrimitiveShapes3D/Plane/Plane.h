#pragma once

#include "3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

class Plane: public BasePrimitiveShape3D {
public:
	Plane(const std::string& textureFilePath = "");
	~Plane()override = default;

	void CreateShape()override;
	void Update()override;

};