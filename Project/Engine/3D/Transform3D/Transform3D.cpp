#include "Transform3D.h"

Transform3D::Transform3D(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Initialize(scale, rotate, translate);
}

Transform3D::Transform3D(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Initialize(scale, rotate, translate);
}

void Transform3D::Initialize(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	scale_ = scale;
	inputRadians_ = rotate;
	rotate_ = MAGIMath::EulerToQuaternionYXZ(inputRadians_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
	// ワールド座標生成
	worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);
}

void Transform3D::Initialize(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	scale_ = scale;
	rotate_ = rotate;
	inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
	translate_ = translate;

	// ワールド行列作成
	worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
	// ワールド座標生成
	worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);
}

void Transform3D::Update() {
	if (isChanged_) {
		// 直接Q回転に変更があった場合はこっち優先
		if (preRotate_ != rotate_) {
			inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);
		} else if (preInputRadians_ != inputRadians_) { 	// ユーザー入力用の回転に変更があった場合
			// 変換してクオータニオンの回転に適用
			rotate_ = MAGIMath::EulerToQuaternionYXZ(inputRadians_);
		}

		// ワールド行列作成
		worldMatrix_ = MAGIMath::MakeAffineMatrix(scale_, rotate_, translate_);
		// ワールド座標生成
		worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);

		// 現在フレームの値を保存
		preRotate_ = rotate_;
		preInputRadians_ = inputRadians_;

		// 次フレーム用のフラグを立てる
		isChanged_ = false;

		// 親がいる場合
		if (parent_) {
			worldMatrix_ = worldMatrix_ * parent_->GetWorldMatrix();
		}

		// もし子がいる場合
		if (!children_.empty()) {
			for (auto& child : children_) {
				if (child) {
					child->SetIsChange(true);
					child->Update();
				}
			}
		}
	}

}

const bool& Transform3D::GetIsChanged() const {
	return isChanged_;
}

const Vector3& Transform3D::GetScale()const {
	return scale_;
}

const Vector3& Transform3D::GetRotate() const {
	return inputRadians_;
}

const Vector3& Transform3D::GetTranslate()const {
	return translate_;
}

const Quaternion& Transform3D::GetQuaternion()const {
	return rotate_;
}

const Matrix4x4& Transform3D::GetWorldMatrix() const {
	return worldMatrix_;
}

const Vector3& Transform3D::GetWorldPosition() const {
	return worldPosition_;
}

void Transform3D::SetParent(Transform3D* parent, bool keepWorld) {
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

	// 変更前のワールド行列を保存
	Matrix4x4 worldBefore = worldMatrix_;

	if (keepWorld && parent_) {
		Matrix4x4 parentWorld = parent_->GetWorldMatrix();
		Matrix4x4 parentInv = MAGIMath::Inverse(parentWorld);

		// 求めたローカル TRS 行列
		Matrix4x4 local = worldBefore * parentInv;

		// 行列を Scale / Quaternion / Translate に分解
		MAGIMath::DecomposeAffineMatrix(local, scale_, rotate_, translate_);
		inputRadians_ = MAGIMath::QuaternionToEuler(rotate_);

		// 新しいワールドを親に掛けて生成し直し
		worldMatrix_ = worldBefore;          // ＝親 * local で一致
		worldPosition_ = MAGIMath::ExtractionWorldPos(worldMatrix_);

		// 子供への Dirty 伝播は不要（見た目は同じ）
		isChanged_ = false;
	} else {
		isChanged_ = true; // 世界を維持しない／親==nullptr の場合は更新させる
	}

	if (isChanged_) {
		// もし子がいる場合
		if (!children_.empty()) {
			for (auto& child : children_) {
				if (child) {
					child->SetIsChange(true);
				}
			}
		}
	}
}

void Transform3D::SetIsChange(bool isChange) {
	isChanged_ = isChange;
}

void Transform3D::SetScale(const Vector3& scale) {
	scale_ = scale;
	isChanged_ = true;
}

void Transform3D::SetRotate(const Vector3& rotate) {
	inputRadians_ = rotate;
	isChanged_ = true;
}

void Transform3D::SetTranslate(const Vector3& translate) {
	translate_ = translate;
	isChanged_ = true;
}

void Transform3D::SetQuaternion(const Quaternion& rotate) {
	rotate_ = rotate;
	isChanged_ = true;
}