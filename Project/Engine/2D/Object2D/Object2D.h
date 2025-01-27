#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "2D/WorldTransform2D/WorldTransform2D.h"
#include "Structs/ObjectStruct.h"
#include "Structs/ModelStruct.h"

/// <summary>
/// 2Dオブジェクトクラス
/// </summary>
class Object2D {
public:
	Object2D(const std::string& objectName, const std::string& textureName);
	~Object2D();
	void Initialize(const std::string& objectName, const std::string& textureName);
	void Update();
	void Draw();



private:


private:
	// トランスフォーム
	WorldTransform2D transform_{};
	// uvTransform
	UVTransform uvTransform_{};

	// WVP用のリソース
	ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// WVPデータ
	Matrix4x4* wvpData_;

	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	// 頂点データ
	VertexData2D* vertexData_ = nullptr;

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	// インデックスデータ
	uint32_t* indexData_ = nullptr;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material2D* materialData_ = nullptr;
	// マテリアル情報を受け取る箱
	Material2D material_;

};