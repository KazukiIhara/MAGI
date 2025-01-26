#include "Object3D.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

Object3D::Object3D(const std::string& objectName, const std::string& modelName)
	:BaseRenderable3D(objectName) {
	Initialize(modelName);
}

Object3D::~Object3D() {
	model_.reset();
}

void Object3D::Initialize(const std::string& modelName) {
	// モデル取得
	model_ = std::make_unique<Model>(MAGISYSTEM::FindModel(modelName));
	assert(model_ && "Warning: Not found model");
}

void Object3D::Update() {
	animationTime_ += MAGISYSTEM::GetDeltaTime();
	animationTime_ = std::fmod(animationTime_, MAGISYSTEM::FindAnimation("Action.002").duration);
	model_->ApplyAnimation(MAGISYSTEM::FindAnimation("Action.002"), animationTime_);

	// モデル更新
	model_->Update();

	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void Object3D::Draw() {
	// 描画用の設定
	PrepareForRendering(model_->IsNormalMap());
	// 3Dモデル描画
	model_->Draw();
}