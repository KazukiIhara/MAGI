#include "GameObject3D.h"

GameObject3D::GameObject3D(const std::string& objectName, const EulerTransform3D& transform)
	:WorldEntity() {
	// 名前をセット
	name_ = objectName;
	// ワールドトランスフォーム作成
	CreateWorldTransform(transform);
}

void GameObject3D::Initialize() {


}

void GameObject3D::Update() {

	if (worldTransform_) {
		worldTransform_->Update();
	}

	if (collider3D_) {
		collider3D_->Update();
	}

	if (renderer3D_) {
		renderer3D_->Update();
	}
}

void GameObject3D::Draw() {

	if (collider3D_) {
		collider3D_->Draw();
	}

	if (renderer3D_) {
		renderer3D_->Draw();
	}
}

WorldTransform* GameObject3D::GetWorldTransform() {
	return worldTransform_.get();
}

void GameObject3D::CreatePrimitiveRenderer(const std::string& rendererName, const Primitive3DType& primitiveType, const std::string& textureName) {
	renderer3D_ = std::make_unique<PrimitiveRenderer3D>(rendererName, primitiveType, textureName);
	renderer3D_->AssignShape();
	renderer3D_->GetWorldTransform()->parent_ = worldTransform_.get();
}

void GameObject3D::CreateStaticRenderer(const std::string& rendererName, const std::string& modelName) {
	renderer3D_ = std::make_unique<StaticRenderer3D>(rendererName, modelName);
	renderer3D_->AssignShape();
	renderer3D_->GetWorldTransform()->parent_ = worldTransform_.get();
}

void GameObject3D::CreateSkinningRenderer(const std::string& rendererName, const std::string& modelName) {
	renderer3D_ = std::make_unique<SkinningRenderer3D>(rendererName, modelName);
	renderer3D_->AssignShape();
	renderer3D_->GetWorldTransform()->parent_ = worldTransform_.get();
}

void GameObject3D::CreateCollider(Collider3DType type) {
	collider3D_ = std::make_unique<Collider3D>(this, type);
}

void GameObject3D::CreateWorldTransform(const EulerTransform3D& transform) {
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = transform.scale;
	worldTransform_->rotate_ = transform.rotate;
	worldTransform_->translate_ = transform.translate;
}