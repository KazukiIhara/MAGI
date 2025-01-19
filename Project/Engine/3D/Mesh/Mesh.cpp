#include "Mesh.h"

#include "Framework/MAGI.h"

Mesh::Mesh(const MeshData& meshData) {
	Initialize(meshData);
}

void Mesh::Initialize(const MeshData& meshData) {
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
