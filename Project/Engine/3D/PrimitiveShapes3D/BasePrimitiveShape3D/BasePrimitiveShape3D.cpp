#include "BasePrimitiveShape3D.h"

#include "Framework/MAGI.h"

BasePrimitiveShape3D::BasePrimitiveShape3D(const std::string& textureFilePath) {
	Initialize(textureFilePath);
}

void BasePrimitiveShape3D::Initialize(const std::string& textureFilePath) {
	// テクスチャをセット
	SetTextureFilePath(textureFilePath);

	// テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる
	if (textureFilePath_ == "") {
		textureFilePath_ = "Engine/Resources/Images/uvChecker.png";
	}

	// 形状を作成
	CreateShape();

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

void BasePrimitiveShape3D::Update() {
	// マテリアルを更新
	materialData_->color = primitiveData_.color;
	materialData_->uvMatrix = primitiveData_.uvMatrix;
}

void BasePrimitiveShape3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// VBVを設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVを設定
	commandList->IASetIndexBuffer(&indexBufferView_);

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[textureFilePath_].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvDescriptorHandleGPU(textureSrvIndex));

	// ModelMaterial用CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, materialResource_->GetGPUVirtualAddress());
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawIndexedInstanced(UINT(primitiveData_.indices.size()), 1, 0, 0, 0);
}

void BasePrimitiveShape3D::SetTextureFilePath(const std::string& textureFilePath) {
	textureFilePath_ = textureFilePath;
}

void BasePrimitiveShape3D::CreateVertexResource() {
	vertexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * primitiveData_.vertices.size());
}

void BasePrimitiveShape3D::CreateVertexBufferView() {
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData3D) * primitiveData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
}

void BasePrimitiveShape3D::MapVertexData() {
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, primitiveData_.vertices.data(), sizeof(VertexData3D) * primitiveData_.vertices.size());
}

void BasePrimitiveShape3D::CreateIndexResource() {
	indexResource_ = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * primitiveData_.indices.size());
}

void BasePrimitiveShape3D::CreateIndexBufferView() {
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * primitiveData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void BasePrimitiveShape3D::MapIndexData() {
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, primitiveData_.indices.data(), sizeof(uint32_t) * primitiveData_.indices.size());
}

void BasePrimitiveShape3D::CreateMaterialResource() {
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(MaterialForGPU));
}

void BasePrimitiveShape3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = primitiveData_.color;
	materialData_->uvMatrix = primitiveData_.uvMatrix;
}
