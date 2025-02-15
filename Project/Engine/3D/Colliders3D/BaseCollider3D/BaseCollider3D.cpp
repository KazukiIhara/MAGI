#include "BaseCollider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseCollider3D::BaseCollider3D(uint32_t id, Collider3DType type) {
	// 識別IDをセット
	id_ = id;
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

uint32_t BaseCollider3D::GetID() const {
	return id_;
}

Vector3& BaseCollider3D::GetOffset() {
	return offset_;
}

bool& BaseCollider3D::GetIsActive() {
	return isActive_;
}

void BaseCollider3D::SetOwner(GameObject3D* owner) {
	// オーナーを登録
	owner_ = owner;
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

