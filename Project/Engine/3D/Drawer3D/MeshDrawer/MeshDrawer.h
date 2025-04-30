#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "Structs/ModelStruct.h"


/// <summary>
/// メッシュ描画クラス
/// </summary>
class MeshDrawer {
public:
	MeshDrawer();
	~MeshDrawer();

	void Update();
	void Draw();

private:
	// 頂点用のリソース
	ComPtr<ID3D12Resource> vertexBuffer_;
	// 頂点数
	uint32_t vertexCount_ = 0;
	// 頂点のsrvインデックス
	uint32_t vertexSrvIndex_ = 0;

	// インデックス用のリソース
	ComPtr<ID3D12Resource> indexBuffer_;
	// インデックス数
	uint32_t indexCount_ = 0;
	// インデックスのsrvインデックス
	uint32_t indexSrvIndex_ = 0;

};