#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "Structs/ModelStruct.h"
#include "DirectX/ComPtr/ComPtr.h"

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh(const MeshData& meshData);
	void Initialize(const MeshData& meshData);
	void Update();
	void Draw();

	void SetTextureFilePath(const std::string& textureFilePath);
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
	ComPtr<ID3D12Resource> vertexResources_;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViews_;

};