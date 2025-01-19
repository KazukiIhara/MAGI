#include "Mesh.h"

#include "Framework/MAGI.h"

Mesh::Mesh(const MeshData& meshData) {
	Initialize(meshData);
}

void Mesh::Initialize(const MeshData& meshData) {
	// メッシュのデータを受けとる
	meshData_ = meshData;

	// TODO:テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる
	if (meshData_.material.textureFilePath == "") {
		meshData_.material.textureFilePath;
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
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	// commandList->SetGraphicsRootDescriptorTable();

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(meshData_.indices.size()), 1, 0, 0, 0);
}

void Mesh::SetTextureFilePath(const std::string& textureFilePath) {
	meshData_.material.textureFilePath = textureFilePath;
}

void Mesh::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * meshData_.vertices.size());
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
