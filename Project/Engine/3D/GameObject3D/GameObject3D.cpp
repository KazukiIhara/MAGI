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



}

void GameObject3D::UpdateWorldTransform() {
	if (worldTransform_) {
		worldTransform_->Update();
	}
}

void GameObject3D::Draw() {

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

void GameObject3D::OnCollisionEnter([[maybe_unused]] BaseCollider3D* other) {

}

void GameObject3D::OnCollisionStay([[maybe_unused]] BaseCollider3D* other) {

}

void GameObject3D::OnCollisionExit([[maybe_unused]] BaseCollider3D* other) {

}

BaseRenderable3D* GameObject3D::GetRenderer3D(const std::string& name) {
	// 描画オブジェクトを検索
	if (!renderers3D_.contains(name)) {
		// エラーを出して止める
		assert(false && "Not Found Renderer3D");
	}
	// 見つかった描画オブジェクトを返す
	return renderers3D_.at(name);
}

BaseCollider3D* GameObject3D::GetCollider3D(const std::string& name) {
	// 作成済みコライダーを検索
	if (!colliders3D_.contains(name)) {
		// エラーメッセージ
		assert(false && "Not Found Collider");
	}
	// コライダーのオフセットの参照を返す
	return colliders3D_.at(name);
}

void GameObject3D::AddRenderer3D(BaseRenderable3D* renderer3D) {
	// nullチェック
	assert(renderer3D && "Add Renderer3D is nullptr");
	// ワールドトランスフォームの親子付け
	renderer3D->GetWorldTransform()->parent_ = worldTransform_.get();
	// 3D描画オブジェクトマップにセット
	renderers3D_.insert(std::pair(renderer3D->name_, renderer3D));
}

void GameObject3D::AddCollider(BaseCollider3D* collider3D) {
	// nullチェック
	assert(collider3D && "Add Collider3D is nullptr");
	// オーナーをセット
	collider3D->SetOwner(this);
	// コライダーマップにセット
	colliders3D_.insert(std::pair(collider3D->name_, collider3D));
}

void GameObject3D::CreateWorldTransform(const EulerTransform3D& transform) {
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = transform.scale;
	worldTransform_->rotate_ = transform.rotate;
	worldTransform_->translate_ = transform.translate;
}