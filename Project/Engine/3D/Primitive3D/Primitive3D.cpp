#include "Primitive3D.h"

#include "3D/PrimitiveShapes3D/Plane/Plane.h"

Primitive3D::Primitive3D(const std::string& objectName, const Primitive3DType& primitiveType)
	:BaseRenderable3D(objectName) {
	Initialize(primitiveType);
}

Primitive3D::~Primitive3D() {
	primitive_.reset();
}

void Primitive3D::Initialize(const Primitive3DType& primitiveType) {
	primitiveType_ = primitiveType;
	switch (primitiveType_) {
		case Primitive3DType::Plane:
			primitive_ = std::make_unique<Plane>();
			break;
		case Primitive3DType::Sphere:
			break;
		case Primitive3DType::Ring:
			break;
		case Primitive3DType::Cylinder:
			break;
	}
}

void Primitive3D::Update() {
	// Primitive更新
	primitive_->Update();
	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void Primitive3D::Draw() {
	// 基底クラスの描画用処理
	BaseRenderable3D::Draw();
	// Primitive描画
	primitive_->Draw();
}
