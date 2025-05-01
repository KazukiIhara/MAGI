#include "MeshDrawer.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "Enums/BlendModeEnum.h"
#include "Framework/MAGI.h"
#include "Logger/Logger.h"

MeshDrawer::MeshDrawer(const MeshData& meshData) {
	// 頂点バッファ作成
	vertexCount_ = static_cast<uint32_t>(meshData.vertices.size());
	assert(vertexCount_ > 0 && "Vertex count must be greater than zero");

	vertexBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * vertexCount_);
	vertexSrvIndex_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIndex_, vertexBuffer_.Get(), vertexCount_, sizeof(VertexData3D));

	// 頂点データをマップ
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, meshData.vertices.data(), sizeof(VertexData3D) * vertexCount_);

	// インデックスバッファ作成
	indexCount_ = static_cast<uint32_t>(meshData.indices.size());
	assert(indexCount_ > 0 && "Index count must be greater than zero");

	indexBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * indexCount_);
	indexSrvIndex_ = MAGISYSTEM::SrvUavAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(indexSrvIndex_, indexBuffer_.Get(), indexCount_, sizeof(uint32_t));

	// インデックスデータをマップ
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, meshData.indices.data(), sizeof(uint32_t) * indexCount_);

	// マテリアルバッファ作成
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(ModelMaterialDataForGPU));
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = {
		.textureIndex = MAGISYSTEM::GetTextureIndex(meshData.material.textureFilePath),
		.baseColor = meshData.material.color,
		.uvMatrix = MAGIMath::MakeIdentityMatrix4x4(),
	};

}

MeshDrawer::~MeshDrawer() {

}

void MeshDrawer::Update() {

}

void MeshDrawer::Draw(uint32_t instanceCount) {
	if (instanceCount == 0) {
		return;
	}

	ID3D12GraphicsCommandList6* commandList = MAGISYSTEM::GetDirectXCommandList6();

	// マテリアルバッファ (b2 : ConstantBufferView)
	commandList->SetGraphicsRootConstantBufferView(2, materialResource_->GetGPUVirtualAddress());

	// 頂点バッファ (t5)
	commandList->SetGraphicsRootDescriptorTable(5, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIndex_));

	// インデックスバッファ (t6)
	commandList->SetGraphicsRootDescriptorTable(6, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(indexSrvIndex_));


	// DispatchMesh呼び出し
	commandList->DispatchMesh(1, instanceCount, 1);

}