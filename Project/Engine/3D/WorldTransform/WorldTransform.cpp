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
	// 回転処理
	if (rotate_.x != preRotate_.x || rotate_.y != preRotate_.y || rotate_.z != preRotate_.z) {
		Rotate();
	}
	// アフィン行列作成
	worldMatrix_ = MakeAffineMatrix(scale_, rotateQuaternion_, translate_);
	// 親がいる場合の計算
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->worldMatrix_;
	}
	// ワールド座標を計算
	worldPosition_ = ExtractionWorldPos(worldMatrix_);
	preRotate_ = rotate_;
}

void WorldTransform::Rotate() {
	// 回転量を取得
	Vector3 deltaRotate = rotate_ - preRotate_;
	// 各軸の回転をクォータニオンに変換
	Quaternion rotationX = MakeRotateAxisAngleQuaternion(MakeRightVector3(), deltaRotate.x);
	Quaternion rotationY = MakeRotateAxisAngleQuaternion(MakeUpVector3(), deltaRotate.y);
	Quaternion rotationZ = MakeRotateAxisAngleQuaternion(MakeForwardVector3(), deltaRotate.z);

	// 回転の合成（順序は必要に応じて変更可能）
	Quaternion deltaRotation = rotationX * rotationY * rotationZ;

	// 現在の回転に適用
	rotateQuaternion_ = deltaRotation * rotateQuaternion_;

	// クォータニオンを正規化
	rotateQuaternion_ = Normalize(rotateQuaternion_);
}