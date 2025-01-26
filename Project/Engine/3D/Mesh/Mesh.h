#pragma once

// C++
#include <string>
#include <cstdint>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Structs/SkinningStruct.h"
#include "Structs/SkeletonStruct.h"

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh(const MeshData& meshData);
	void Initialize(const MeshData& meshData);
	void Update();
	void Draw();

	// スキニング
	void Skinning(const uint32_t& paletteSrvIndex);

	// 影響度の参照を渡す
	std::span<VertexInfluence>& GetMappdInfluence();

	bool IsNormalMap() const;
private:
	// 頂点リソースの作成
	void CreateVertexResource();
	// 頂点バッファービューの作成
	void CreateVertexBufferView();
	// 頂点データの書き込み
	void MapVertexData();

	// インデックスリソースの作成
	void CreateIndexResource();
	// インデックスバッファビューの作成
	void CreateIndexBufferView();
	// インデックスデータの書き込み
	void MapIndexData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();

	//
	// forSkinning
	//

	// スキニング用頂点リソースの作成
	void CreateSkinningVertexResources();
	// スキニング用頂点バッファビュー
	void CreateSkinningVertexBufferView();
	// スキニング情報用のリソース作成
	void CreateSkinningInformationResource();
	// スキニング情報用のデータ書き込み
	void MapSkinningInformationData();

	// スキニング影響度用のリソースを作成
	void CreateInfluenceResource();

private:
	// メッシュデータ
	MeshData meshData_{};

	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// 頂点データ
	VertexData3D* vertexData_ = nullptr;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// インデックスデータ
	uint32_t* indexData_ = nullptr;
	// IBV
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	MaterialForGPU* materialData_ = nullptr;


	//
	// forSkinning
	//

	// スキニング用頂点リソース
	ComPtr<ID3D12Resource> skinningVertexResource_;
	// スキニング用VBV
	D3D12_VERTEX_BUFFER_VIEW skinningVertexBufferView_;

	// スキニング影響度のリソース
	ComPtr<ID3D12Resource> influenceResource_;
	// スキニング影響度
	std::span<VertexInfluence> mappedInfluence_;

	// スキニング用の情報リソース
	ComPtr<ID3D12Resource> skinningInformationResource_;
	// スキニング用の情報データ
	SkinningInformationForGPU* skiningInformationData_ = nullptr;


	// スキニングに必要なリソースインデックス
	uint32_t vertexSrvIndex_;
	uint32_t vertexUavIndex_;
	uint32_t influenceSrvIndex;
};