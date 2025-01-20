#include "Object3D.h"

#include <cassert>

#include "Framework/MAGI.h"

Object3D::Object3D(const std::string& modelName) {
	Initialize(modelName);
}

Object3D::~Object3D() {
	model_.reset();
}

void Object3D::Initialize(const std::string& modelName) {
	worldTransform_.Initialize();
	model_ = std::make_unique<Model>();
	assert(model_);
}

void Object3D::Update() {
	model_->Update();
	worldTransform_.Update();
}

void Object3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3D, blendMode_));
	// マテリアルCBufferの場所を設定

	// wvp用のCBufferの場所を設定

	// ライトを転送

	// ライトの総数を転送

	// カメラ情報を転送

	// 3Dモデル描画
	model_->Draw();
}

void Object3D::CreateWVPResource() {

}

void Object3D::MapWVPData() {

}

void Object3D::CreateMaterialResource() {

}

void Object3D::MapMateiralData() {

}
