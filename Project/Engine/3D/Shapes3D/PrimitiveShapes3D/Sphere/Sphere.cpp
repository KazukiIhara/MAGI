#include "Sphere.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Sphere::Sphere(const std::string& textureName)
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize(textureName);
}

void Sphere::SetShape() {
	primitiveData_ = MAGISYSTEM::GetPrimitiveShape(Primitive3DType::Sphere);
}

void Sphere::Update() {
	BasePrimitiveShape3D::Update();
}
