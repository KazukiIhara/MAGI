#include "BaseCollider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseCollider3D::BaseCollider3D(Collider3DType type) {
	// コライダーのタイプを登録
	type_ = type;
	// トランスフォームを初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
}

void BaseCollider3D::Update() {
	// ワールド行列の更新
	worldTransform_->translate_ = offset_;
	worldTransform_->Update();
	worldPosition_ = ExtractionWorldPos(worldTransform_->worldMatrix_);
}

Collider3DType BaseCollider3D::GetType() const {
	return type_.value();
}

Vector3& BaseCollider3D::GetOffset() {
	return offset_;
}

void BaseCollider3D::SetOwner(GameObject3D* owner) {
	// オーナーを登録
	owner_ = owner;
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

