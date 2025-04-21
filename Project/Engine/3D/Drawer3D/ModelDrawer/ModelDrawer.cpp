#include "ModelDrawer.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"

ModelDrawer::ModelDrawer(const ModelData& modelData) {
	modelData;
}

ModelDrawer::~ModelDrawer() {

}

void ModelDrawer::AddDrawCommand(const Matrix4x4& worldMatrix) {
	worldMatrix;
}

void ModelDrawer::Update() {
	// 各メッシュの更新
	for (auto& mesh : meshes_) {
		mesh->Update();
	}
}

void ModelDrawer::Draw() {
	// 各メッシュの描画
	for (auto& mesh : meshes_) {
		mesh->Draw();
	}
}