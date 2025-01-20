#include "WorldTransform.h"

using namespace MAGIMath;

void WorldTransform::Initialize() {
	WorldEntity::Initialize();
	rotate_ = { 0.0f,0.0f,0.0f };

	scale_ = { 1.0f,1.0f,1.0f };
	rotateQuaternion_ = MakeIdentityQuaternion();
	translate_ = { 0.0f,0.0f,0.0f };
	worldMatrix_ = MakeIdentityMatrix4x4();
}

void WorldTransform::Update() {
	// オイラー角をクオータニオンに変換
	rotateQuaternion_ = EulerToQuaternion(rotate_);
	// アフィン行列作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotateQuaternion_, translate_);
	// 親がいる場合の計算
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
	// ワールド座標を計算
	worldPosition_ = ExtractionWorldPos(worldMatrix_);
}
