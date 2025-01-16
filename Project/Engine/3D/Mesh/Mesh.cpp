#include "Mesh.h"

Mesh::Mesh(const MeshData& meshData, const std::string& textureFilePath) {
	Initialize(meshData, textureFilePath);
}

void Mesh::Initialize(const MeshData& meshData, const std::string& textureFilePath) {
	meshData_ = meshData;
}

void Mesh::Update() {

}

void Mesh::Draw() {

}

void Mesh::SetTextureFilePath(const std::string& textureFilePath) {
	meshData_.material.textureFilePath = textureFilePath;
}

void Mesh::CreateVertexResource() {
	
}

void Mesh::CreateVertexBufferView() {
	
}

void Mesh::MapVertexData() {
	
}

void Mesh::CreateIndexResource() {
	
}

void Mesh::CreateIndexBufferView() {
	
}

void Mesh::MapIndexData() {
	
}

void Mesh::CreateMaterialResource() {
	
}

void Mesh::MapMaterialData() {
	
}
