#pragma once
// DirectX
#include <d3d12.h>

// DirectXMesh
#include <DirectXMesh/DirectXMesh.h>

// MAGI
#include "DirectX/ComPtr/ComPtr.h"
#include "Structs/ModelStruct.h"
#include "Structs/Primitive3DStruct.h"

/// <summary>メッシュ描画クラス（Mesh Shader + Meshlet）</summary>
class MeshDrawer {
public:
	explicit MeshDrawer(const MeshData& meshData);
	~MeshDrawer();

	void Update();
	void Draw(uint32_t instanceCount);

private:
	/* ---------- 頂点 / インデックス ---------- */
	ComPtr<ID3D12Resource> vertexBuffer_;
	VertexData3D* vertexData_ = nullptr;
	uint32_t vertexCount_ = 0;
	uint32_t vertexSrvIdx_ = 0;

	ComPtr<ID3D12Resource> indexBuffer_;
	uint32_t* indexData_ = nullptr;
	uint32_t indexCount_ = 0;
	uint32_t indexSrvIdx_ = 0;

	/* ---------- Meshlet ---------- */
	ComPtr<ID3D12Resource> meshletBuffer_;              // StructuredBuffer<Meshlet>
	uint32_t meshletCount_ = 0;
	uint32_t meshletSrvIdx_ = 0;

	ComPtr<ID3D12Resource> meshletUniqueVertIB_;        // StructuredBuffer
	uint32_t uniqueVertCount_ = 0;
	uint32_t uniqueVertSrvIdx_ = 0;

	ComPtr<ID3D12Resource> meshletPrimIB_;              // StructuredBuffer<MeshletTriangle>
	uint32_t primSrvIdx_ = 0;

	/* ---------- マテリアル ---------- */
	ComPtr<ID3D12Resource> materialBuffer_;
	ModelMaterialDataForGPU* material_ = nullptr;
};
