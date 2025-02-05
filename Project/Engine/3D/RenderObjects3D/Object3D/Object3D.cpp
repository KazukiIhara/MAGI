#include "Object3D.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

Object3D::Object3D(const std::string& objectName, const std::string& modelName)
	:BaseRenderable3D(objectName) {
	modelData_ = MAGISYSTEM::FindModel(modelName);
}

Object3D::~Object3D() {
	model_.reset();
}

void Object3D::AssignShape() {
	// モデル取得
	model_ = std::make_unique<Model>(modelData_);
	model_->Initialize();
	assert(model_ && "Warning: Not found model");
}

void Object3D::Update() {
	// モデル更新
	model_->Update();

	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void Object3D::Draw() {
	// 描画用の設定
	PrepareForRendering();
	// 3Dモデル描画
	model_->Draw();
}