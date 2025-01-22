#pragma once

#include "3D/WorldEntity/WorldEntity.h"

class WorldTransform :public WorldEntity {
public:
	void Initialize()override;
	void Update();
private:
	void UpdateEuler();
	void UpdateQuaternion();
	void RotateQuaternion();
public:
	// クオータニオン角を使うかどうか
	bool isUseQuaternion_ = false;
	// ローカルトランスフォーム
	Vector3 scale_ = { 1.0f,1.0f,1.0f };// 拡縮量
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };// 回転量
	Vector3 translate_ = { 0.0f,0.0f,0.0f };// 移動量

	// 親
	WorldTransform* parent_ = nullptr;
	// ワールド行列
	Matrix4x4 worldMatrix_{};
private:

	// クオータニオン計算用
	Vector3 preRotate_ = { 0.0f,0.0f,0.0f };// 回転量
	Quaternion rotateQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };// 回転量
};