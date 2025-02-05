#pragma once

#include "3D/Shapes3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"

class Plane : public BasePrimitiveShape3D {
public:
	Plane(const std::string& textureName = "");
	~Plane()override = default;
	void SetShape()override;
	void Update()override;
private:

};