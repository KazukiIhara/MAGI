#include "Model.h"

#include "Framework/MAGI.h"

#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIMath;
using namespace MAGIUtility;

Model::Model(const ModelData& modeldata) {
	Initialize(modeldata);
}

Model::~Model() {
	
}

void Model::Initialize(const ModelData& modeldata) {
	modelData_ = modeldata;
	CreateMehes();
}

void Model::Update() {	
	// 各メッシュの更新
	for (Mesh mesh : meshes_) {
		mesh.Update();
	}
}

void Model::Draw() {
	for (Mesh mesh : meshes_) {
		mesh.Draw();
	}
}

bool Model::IsNormalMap() {
	return meshes_[0].IsNormalMap();
}

void Model::CreateMehes() {
	for (uint32_t i = 0; i < modelData_.meshes.size(); i++) {
		Mesh newMesh(modelData_.meshes[i]);
		meshes_.push_back(newMesh);
	}
}
