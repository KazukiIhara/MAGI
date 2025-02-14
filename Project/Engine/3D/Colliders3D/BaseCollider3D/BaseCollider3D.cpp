#include "BaseCollider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseCollider3D::BaseCollider3D(GameObject3D* owner, Collider3DType type) {
	// オーナーを登録
	owner_ = owner;
	// コライダーのタイプを登録
	type_ = type;
	// トランスフォームを初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

void BaseCollider3D::Update() {
	// ワールド行列の更新
	worldTransform_->Update();
	worldPosition_ = ExtractionWorldPos(worldTransform_->worldMatrix_);
}

Collider3DType BaseCollider3D::GetType() const {
	return type_.value();
}

