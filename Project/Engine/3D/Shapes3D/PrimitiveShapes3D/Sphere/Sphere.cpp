#include "Sphere.h"
using namespace MAGIMath;

Sphere::Sphere(const std::string& textureFilePath)
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize(textureFilePath);
}

void Sphere::CreateShape() {

}

void Sphere::Update() {
	BasePrimitiveShape3D::Update();
}
