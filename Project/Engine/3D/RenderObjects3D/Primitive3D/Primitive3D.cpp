#include "Primitive3D.h"

#include "3D/Shapes3D/PrimitiveShapes3D/Plane/Plane.h"
#include "3D/Shapes3D/PrimitiveShapes3D/Sphere/Sphere.h"

Primitive3D::Primitive3D(const std::string& objectName, const Primitive3DType& primitiveType, const std::string& textureFilePath)
	:BaseRenderable3D(objectName) {
	Initialize(primitiveType, textureFilePath);
}

Primitive3D::~Primitive3D() {
	primitive_.reset();
}

void Primitive3D::Update() {
	// Primitive更新
	primitive_->Update();
	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void Primitive3D::Draw() {
	// 描画の設定
	PrepareForRendering();
	// Primitive描画
	primitive_->Draw();
}

void Primitive3D::Initialize(const Primitive3DType& primitiveType, const std::string& textureFilePath) {
	primitiveType_ = primitiveType;
	switch (primitiveType_) {
	case Primitive3DType::Plane:
		primitive_ = std::make_unique<Plane>(textureFilePath);
		break;
	case Primitive3DType::Sphere:
		primitive_ = std::make_unique<Sphere>(textureFilePath);
		break;
	case Primitive3DType::Ring:
		break;
	case Primitive3DType::Cylinder:
		break;
	}
}