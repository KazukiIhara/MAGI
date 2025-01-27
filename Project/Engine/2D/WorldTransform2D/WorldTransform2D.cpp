#include "WorldTransform2D.h"

using namespace MAGIMath;

void WorldTransform2D::Initialize() {
	// 値を初期化
	size_ = { 256.0f,256.0f };
	rotate_ = 0.0f;
	position = { 0.0f,0.0f };
	Vector3 scale = { size_.x,size_.y,0.0f };
	Vector3 rotate = { 0.0f,0.0f,rotate_ };
	Vector3 translate = { position.x,position.y,0.0f };
	// 行列を初期化
	worldMatrix_ = MakeAffineMatrix(scale, rotate, translate);
}

void WorldTransform2D::Update() {
	// 4x4行列作成用に一時変数に挿入
	Vector3 scale = { size_.x,size_.y,0.0f };
	Vector3 rotate = { 0.0f,0.0f,rotate_ };
	Vector3 translate = { position.x,position.y,0.0f };
	// トランスフォームから行列を作る
	worldMatrix_ = MakeAffineMatrix(scale, rotate, translate);
}
