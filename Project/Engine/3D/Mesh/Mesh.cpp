#include "Mesh.h"

#include "Framework/MAGI.h"

Mesh::Mesh(const MeshData& meshData) {
	Initialize(meshData);
}

void Mesh::Initialize(const MeshData& meshData) {
	// メッシュのデータを受けとる
	meshData_ = meshData;

	// テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる
	if (meshData_.material.textureFilePath == "") {
		meshData_.material.textureFilePath = "Engine/Resources/Images/uvChecker.png";
	}

	// 頂点リソースの作成
	CreateVertexResource();
	// 頂点バッファビューの作成
	CreateVertexBufferView();
	// 頂点データの書き込み
	MapVertexData();

	// 描画用のインデックスリソースを作成
	CreateIndexResource();
	// インデックスバッファビューの作成
	CreateIndexBufferView();
	// インデックスリソースにデータを書き込む
	MapIndexData();

	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルにデータを書き込む
	MapMaterialData();

	//
	// forSkinning
	//

	// スキニング影響度のリソース作成
	CreateInfluenceResource();

	// スキニング用の頂点リソースを作成
	CreateSkinningVertexResources();
	// スキニング用の頂点バッファビューを作成
	CreateSkinningVertexBufferView();
	// スキニング用情報のリソースを作成
	CreateSkinningInformationResource();
	// スキニング情報用のデータを書き込み
	MapSkinningInformationData();


}

void Mesh::Update() {
	// マテリアルを更新
	materialData_->color = meshData_.material.color;
	materialData_->uvMatrix = meshData_.material.uvMatrix;
}

void Mesh::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &skinningVertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.textureFilePath].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvDescriptorHandleGPU(textureSrvIndex));

	// NormalMap用のSrvをセット
	if (meshData_.material.normalMapTextureFilePath != "") {
		uint32_t normalMapTextureSrvIndex = MAGISYSTEM::GetTexture()[meshData_.material.normalMapTextureFilePath].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvDescriptorHandleGPU(normalMapTextureSrvIndex));
	}

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), 1, 0, 0, 0);
}

std::span<VertexInfluence>& Mesh::GetMappdInfluence() {
	return mappedInfluence_;
}

bool Mesh::IsNormalMap() const {
	return meshData_.material.normalMapTextureFilePath != "";
}

void Mesh::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size());

	//
	// forSkinning
	//

	vertexSrvIndex_ = MAGISYSTEM::ViewAllocate();
	MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIndex_, vertexResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexData3D));
}

void Mesh::CreateVertexBufferView() {
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * meshData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void Mesh::MapVertexData() {
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, meshData_.vertices.data(), sizeof(VertexData3D) * meshData_.vertices.size());
}

void Mesh::CreateIndexResource() {
	indexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * meshData_.indices.size());
}

void Mesh::CreateIndexBufferView() {
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * meshData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void Mesh::MapIndexData() {
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, meshData_.indices.data(), sizeof(uint32_t) * meshData_.indices.size());
}

void Mesh::CreateMaterialResource() {
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(MaterialForGPU));
}

void Mesh::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = meshData_.material.color;
	materialData_->uvMatrix = meshData_.material.uvMatrix;
}

void Mesh::CreateSkinningVertexResources() {
	skinningVertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size(), true);
	vertexUavIndex_ = MAGISYSTEM::ViewAllocate();
	MAGISYSTEM::CreateUavStructuredBuffer(vertexUavIndex_, skinningVertexResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexData3D));
}

void Mesh::CreateSkinningVertexBufferView() {
	skinningVertexBufferView_.BufferLocation = skinningVertexResource_->GetGPUVirtualAddress();
	skinningVertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * meshData_.vertices.size());
	skinningVertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void Mesh::CreateSkinningInformationResource() {
	skinningInformationResource_ = nullptr;
	skinningInformationResource_ = MAGISYSTEM::CreateBufferResource(sizeof(SkinningInformationForGPU));
}

void Mesh::MapSkinningInformationData() {
	skiningInformationData_ = nullptr;
	skinningInformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&skiningInformationData_));
	skiningInformationData_->numVertices = static_cast<uint32_t>(meshData_.vertices.size());
}

void Mesh::CreateInfluenceResource() {
	// リソースを確保
	influenceResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexInfluence) * meshData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * meshData_.vertices.size());
	mappedInfluence_ = { mappedInfluence,meshData_.vertices.size() };
	// インデックス割り当て
	influenceSrvIndex = MAGISYSTEM::ViewAllocate();
	// srv作成
	MAGISYSTEM::CreateSrvStructuredBuffer(influenceSrvIndex, influenceResource_.Get(), static_cast<uint32_t>(meshData_.vertices.size()), sizeof(VertexInfluence));
}



void Mesh::Skinning(const uint32_t& paletteSrvIndex) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	commandList->SetComputeRootSignature(MAGISYSTEM::GetComputeRootSignature(ComputePipelineStateType::Skinning));
	commandList->SetPipelineState(MAGISYSTEM::GetCompurePipelineState(ComputePipelineStateType::Skinning));

	// DescriptoorHeapの設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { MAGISYSTEM::GetSrvUavDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// コマンドを積む
	commandList->SetComputeRootDescriptorTable(0, MAGISYSTEM::GetSrvDescriptorHandleGPU(paletteSrvIndex));
	commandList->SetComputeRootDescriptorTable(1, MAGISYSTEM::GetSrvDescriptorHandleGPU(vertexSrvIndex_));
	commandList->SetComputeRootDescriptorTable(2, MAGISYSTEM::GetSrvDescriptorHandleGPU(influenceSrvIndex));
	commandList->SetComputeRootDescriptorTable(3, MAGISYSTEM::GetSrvDescriptorHandleGPU(vertexUavIndex_));
	commandList->SetComputeRootConstantBufferView(4, skinningInformationResource_->GetGPUVirtualAddress());

	// コマンド発行
	commandList->Dispatch(UINT(meshData_.vertices.size() + 1023) / 1024, 1, 1);

	// コマンド実行
	MAGISYSTEM::KickCommand();
	MAGISYSTEM::WaitGPU();
	MAGISYSTEM::ResetCommand();

}
