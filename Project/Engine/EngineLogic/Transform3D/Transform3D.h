#pragma once

// 数学ヘッダ
#include "Math/Utility/MathUtility.h"

/// <summary>
/// 3D空間トランスフォーム
/// </summary>
class Transform3D {
public:
	Transform3D(
		const Vector3& scale = { 1.0f,1.0f,1.0f },
		const Vector3& rotate = { 0.0f,0.0f,0.0f },
		const Vector3& translate = { 0.0f,0.0f,0.0f }
	);

	Transform3D(
		const Vector3& scale,
		const Quaternion& rotate,
		const Vector3& translate
	);

	~Transform3D() = default;

	void Update();

	// 親をセット
	void SetParent(Transform3D* parent);


	//
	// ゲッター(参照渡しも含む)
	//

	Vector3& GetScale();
	Vector3& GetRotate();
	Vector3& GetTranslate();

	const Matrix4x4& GetWorldMatrix()const;

private:
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Quaternion rotate_ = { 0.0f,0.0f,0.0f,1.0f };
	Vector3 translate_ = { 0.0f,0.0f,0.0f };

	Vector3 inputRadians_ = { 0.0f,0.0f,0.0f };

	Matrix4x4 worldMatrix_;

	// 親
	Transform3D* parent_ = nullptr;
	// 子
	std::vector<Transform3D*> children_;

private:
	// コピー禁止
    Transform3D(const Transform3D&) = delete;
    Transform3D& operator=(const Transform3D&) = delete;
};
