#include "GameObject3D.h"

GameObject3D::GameObject3D(const std::string& objectName)
	:WorldEntity() {
	// 名前をセット
	name_ = objectName;


}

void GameObject3D::Initialize(const EulerTransform3D& transform) {
	CreateWorldTransform(transform);
}

void GameObject3D::Update() {

	if (worldTransform_) {
		worldTransform_->Update();
	}

	if (renderer3D_) {
		renderer3D_->Update();
	}
}

void GameObject3D::Draw() {

	if (renderer3D_) {
		renderer3D_->Draw();
	}
}

WorldTransform* GameObject3D::GetWorldTransform() {
	return worldTransform_.get();
}

void GameObject3D::CreateWorldTransform(const EulerTransform3D& transform) {
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = transform.scale;
	worldTransform_->rotate_ = transform.rotate;
	worldTransform_->translate_ = transform.translate;
}

void GameObject3D::CreatePrimitiveRenderer() {}

void GameObject3D::CreateStaticRenderer() {}

void GameObject3D::CreateSkinningRenderer() {}
