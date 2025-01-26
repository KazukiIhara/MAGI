#pragma once

// C++
#include <string>
#include <vector>
#include <memory>

#include "3D/Mesh/Mesh.h"
#include "3D/Skeleton/Skeleton.h"
#include "Structs/AnimationStruct.h"

class Model {
public:
	Model(const ModelData& modeldata);
	~Model();

	void Initialize(const ModelData& modeldata);
	void Update();
	void Draw();

	//
	// forSkinning
	//
	
	// アニメーションの適用
	void ApplyAnimation(const AnimationData& animation, float animationTime);

	bool IsNormalMap();
private:
	// 受け取ったモデルからメッシュを作成
	void CreateMehes();

	// 
	// forSkinning
	// 

	// パレットのリソース作成
	void CreateSkinPaletteResource();

	// InverseBindPoseMatrixの作成
	void CreateInverseBindPoseMatrix();

	// 全メッシュにスキニング影響度を設定
	void SettingInfluenceAllMeshes();

	// パレットの更新
	void SkinPaletteUpdate();
private:
	// 受け取るモデルデータ
	ModelData modelData_{};
	// メッシュ
	std::vector<Mesh> meshes_;

	// 
	// forSkinning
	// 

	// スケルトン
	std::unique_ptr<Skeleton> skeleton_ = nullptr;

	// パレットのリソース
	ComPtr<ID3D12Resource> paletteResource_ = nullptr;
	std::span<WellForGPU> mappedPalette_;
	uint32_t paletteSrvIndex_;

	// 逆バインドポーズ
	std::vector<Matrix4x4> inverseBindPoseMatrices_;
};