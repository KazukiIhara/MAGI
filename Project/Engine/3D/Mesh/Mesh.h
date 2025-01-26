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

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh(const MeshData& meshData);
	void Initialize(const MeshData& meshData);
	void Update();
	void Draw();

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

	// スキニング影響度のリソース
	ComPtr<ID3D12Resource> influenceResource_;

	// スキニングに必要なリソースインデックス
	uint32_t vertexSrvIndex_;
	uint32_t vertexUavIndex_;
	uint32_t influenceSrvIndex;

	std::span<VertexInfluence> mappedInfluence;
};