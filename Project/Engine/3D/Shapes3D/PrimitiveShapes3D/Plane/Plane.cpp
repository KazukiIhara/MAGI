#include "Plane.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Plane::Plane(const std::string& textureFilePath)
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize(textureFilePath);
}

void Plane::SetShape() {
	primitiveData_ = MAGISYSTEM::GetPrimitiveShape(Primitive3DType::Plane);
}

void Plane::Update() {
	BasePrimitiveShape3D::Update();
}
