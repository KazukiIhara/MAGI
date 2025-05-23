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

}

void Transform3D::AddChild(Transform3D* chaild) {

}
