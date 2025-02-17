#include "GameObject3D.h"
#include "MAGIUitility/MAGIUtility.h"
#include <cassert>

using namespace MAGIUtility;

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

	if (renderer3D_) {
		renderer3D_->Update();
	}
}

void GameObject3D::Draw() {

	if (renderer3D_) {
		renderer3D_->Draw();
	}
}

Vector3& GameObject3D::GetScale() {
	return worldTransform_->scale_;
}

Vector3& GameObject3D::GetRotate() {
	return worldTransform_->rotate_;
}

Vector3& GameObject3D::GetTranslate() {
	return worldTransform_->translate_;
}

WorldTransform* GameObject3D::GetWorldTransform() {
	return worldTransform_.get();
}

void GameObject3D::OnCollisionEnter([[maybe_unused]] GameObject3D* other) {

}

void GameObject3D::OnCollisionStay([[maybe_unused]] GameObject3D* other) {

}

void GameObject3D::OnCollisionExit([[maybe_unused]] GameObject3D* other) {

}

bool& GameObject3D::GetColliderIsActive(const std::string& name) {
	// 作成済みコライダーを検索
	if (colliders3D_.contains(name)) {
		// アクティブフラグの参照を返す
		return colliders3D_.at(name)->GetIsActive();
	} else {
		// エラーメッセージ
		assert(false && "Not Found Collider");
		return colliders3D_.at(name)->GetIsActive();
	}
}

Vector3& GameObject3D::GetColliderOffset(const std::string& name) {
	// 作成済みコライダーを検索
	if (colliders3D_.contains(name)) {
		// コライダーを返す
		return colliders3D_.at(name)->GetOffset();
	}
	// エラーメッセージ
	assert(false && "Not Found Collider");
	// コライダーを返す
	return colliders3D_.at(name)->GetOffset();
}

void GameObject3D::SetColliderRadius(const std::string& name, float radius) {
	// 作成済みコライダーを検索
	if (colliders3D_.contains(name)) {
		// 球体コライダーかどうかチェック
		if (auto it = dynamic_cast<SphereCollider*>(colliders3D_.at(name))) {
			// 半径を設定
			it->GetRadius() = radius;
		} else {
			assert(false && "Not Sphere Collider");
		}
	}
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

void GameObject3D::AddCollider(BaseCollider3D* collider) {
	// nullチェック
	assert(collider);
	// オーナーをセット
	collider->SetOwner(this);
	// コライダーマップにセット
	colliders3D_.insert(std::pair(collider->name_, collider));
}

void GameObject3D::CreateWorldTransform(const EulerTransform3D& transform) {
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = transform.scale;
	worldTransform_->rotate_ = transform.rotate;
	worldTransform_->translate_ = transform.translate;
}