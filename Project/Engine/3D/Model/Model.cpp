#include "Model.h"

#include "Framework/MAGI.h"

Model::Model(const ModelData& modeldata) {
	Initialize(modeldata);
}

Model::~Model() {
	skeleton_.reset();
}

void Model::Initialize(const ModelData& modeldata) {
	modelData_ = modeldata;
	CreateMehes();
	skeleton_ = std::make_unique<Skeleton>(modelData_.rootNode, modelData_.inverseBindPoseMatrices);
}

void Model::Update() {
	// パレットの更新
	SkinPaletteUpdate();
	// 各メッシュの更新
	for (Mesh mesh : meshes_) {
		mesh.Update();

		//mesh.Skinning(paletteSrvIndex_);
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

void Model::CreateSkinPaletteResource() {
	// palette用のリソースを確保
	paletteResource_ = MAGISYSTEM::CreateBufferResource(sizeof(WellForGPU) * skeleton_->jointMap.size());
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette,skeleton_->joints.size() };

	// srvのインデックスを割り当て
	paletteSrvIndex_ = MAGISYSTEM::ViewAllocate();

	// srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(paletteSrvIndex_, paletteResource_.Get(), UINT(skeleton_->joints.size()), sizeof(WellForGPU));
}

void Model::SkinPaletteUpdate() {

}

void Model::Skinning() {
	for (Mesh mesh : meshes_) {
		
	}
}
