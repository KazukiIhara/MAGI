#pragma once

#include "3D/WorldEntity/WorldEntity.h"

class WorldTransform :public WorldEntity {
public:
	void Initialize()override;
	void Update();
public:
	void Rotate();
	// ローカルトランスフォーム
	Vector3 scale_ = { 1.0f,1.0f,1.0f };// 拡縮量
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };// 回転量(入力用)
	Vector3 translate_ = { 0.0f,0.0f,0.0f };// 移動量

	Quaternion rotateQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };// 回転量

	// 親
	WorldTransform* parent_ = nullptr;

	// ワールド行列
	Matrix4x4 worldMatrix_{};
private:
	Vector3 preRotate_ = { 0.0f,0.0f,0.0f };// 回転量(保存用)
};