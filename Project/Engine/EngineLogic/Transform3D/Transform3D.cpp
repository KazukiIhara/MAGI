#include "Transform3D.h"

Transform3D::Transform3D(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	scale_ = scale;
	inputRadians_ = rotate;
	rotate_ = MAGIMath::EulerToQuaternionXYZ(inputRadians_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
}

Transform3D::Transform3D(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	scale_ = scale;
	rotate_ = rotate;
	inputRadians_ = MAGIMath::QuaternionToEulerXYZ(rotate_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
}

void Transform3D::Update() {
	// ラジアンの回転をクオータニオンに変換
	rotate_ = MAGIMath::EulerToQuaternionXYZ(inputRadians_);

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);

	// 親がいる場合
	if (parent_) {
		worldMatrix_ = worldMatrix_ * parent_->GetWorldMatrix();
	}

	// もし子がいる場合
	if (!children_.empty()) {
		for (auto& child : children_) {
			if (child) {
				child->Update();
			}
		}
	}

}

Vector3& Transform3D::GetScale() {
	return scale_;
}

Vector3& Transform3D::GetRotate() {
	return inputRadians_;
}

Vector3& Transform3D::GetTranslate() {
	return translate_;
}

const Matrix4x4& Transform3D::GetWorldMatrix() const {
	return worldMatrix_;
}

void Transform3D::SetParent(Transform3D* parent) {
	// 同じだった場合は早期リターン
	if (parent_ == parent) {
		return;
	}

	// すでに親がいた場合は旧親から自身を削除する
	if (parent_) {
		auto& oldChildren = parent_->children_;
		oldChildren.erase(
			std::remove(oldChildren.begin(), oldChildren.end(), this),
			oldChildren.end()
		);
	}

	// 親をセット
	parent_ = parent;

	// 親に自身を子としてセット
	if (parent_) {
		parent_->children_.push_back(this);
	}
}
