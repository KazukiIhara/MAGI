#include "Collider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

Collider3D::Collider3D(GameObject3D* owner, Collider3DType type) {
	owner_ = owner;
	type_ = type;
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

Collider3D::~Collider3D() {}

void Collider3D::Update() {

}

void Collider3D::Draw() {

}
