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
	// モデル更新
	model_->Update();
	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void Object3D::Draw() {
	// 基底クラスの描画用処理
	BaseRenderable3D::Draw();
	// 3Dモデル描画
	model_->Draw();
}