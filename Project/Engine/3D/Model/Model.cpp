#include "Model.h"

#include "Framework/MAGI.h"

#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIMath;
using namespace MAGIUtility;

Model::Model(const ModelData& modeldata) {
	Initialize(modeldata);
}

Model::~Model() {
	skeleton_.reset();
}

void Model::Initialize(const ModelData& modeldata) {
	modelData_ = modeldata;
	CreateMehes();
	skeleton_ = std::make_unique<Skeleton>(modelData_.rootNode);
	CreateSkinPaletteResource();
	CreateInverseBindPoseMatrix();
	SettingInfluenceAllMeshes();
}

void Model::Update() {
	// スケルトンの更新
	skeleton_->Update();
	// パレットの更新
	SkinPaletteUpdate();
	// 各メッシュの更新
	for (Mesh mesh : meshes_) {
		mesh.Update();

		mesh.Skinning(paletteSrvIndex_);
	}
}

void Model::Draw() {
	for (Mesh mesh : meshes_) {
		mesh.Draw();
	}
}

void Model::ApplyAnimation(const AnimationData& animation, float animationTime) {
	for (Joint& joint : skeleton_->joints) {
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateVelue(rootNodeAnimation.translate, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
			joint.transform.scale = CalculateVelue(rootNodeAnimation.scale, animationTime);
		}
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

void Model::CreateInverseBindPoseMatrix() {
	// InverseBindPoseMatrixの保存領域を作成
	inverseBindPoseMatrices_.resize(skeleton_->joints.size());
	for (size_t i = 0; i < skeleton_->joints.size(); i++) {
		inverseBindPoseMatrices_[i] = MakeIdentityMatrix4x4();
	}

	for (const auto& jointWeight : modelData_.skinClusterData) {
		auto it = skeleton_->jointMap.find(jointWeight.first);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}
		inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;

	}
}

void Model::SettingInfluenceAllMeshes() {
	for (const auto& jointWeight : modelData_.skinClusterData) {
		auto it = skeleton_->jointMap.find(jointWeight.first);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}

		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			for (auto& mesh : meshes_) {

				auto& currentInfluence = mesh.GetMappdInfluence()[vertexWeight.vertexIndex];
				for (uint32_t index = 0; index < kNumMaxInfluence; index++) {
					if (currentInfluence.weights[index] == 0.0f) {
						currentInfluence.weights[index] = vertexWeight.weight;
						currentInfluence.jointIndices[index] = (*it).second;
						break;
					}
				}
			}


		}

	}
}

void Model::SkinPaletteUpdate() {
	for (size_t jointIndex = 0; jointIndex < skeleton_->joints.size(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices_.size());
		mappedPalette_[jointIndex].skeletonSpaceMatrix =
			inverseBindPoseMatrices_[jointIndex] * skeleton_->joints[jointIndex].skeletonSpaceMatrix;
		mappedPalette_[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
	}
}
