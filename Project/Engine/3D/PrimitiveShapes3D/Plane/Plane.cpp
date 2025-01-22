#include "Plane.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Plane::Plane(const std::string& textureFilePath)
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize(textureFilePath);
}

void Plane::CreateShape() {

	// 頂点データの設定
	primitiveData_.vertices.push_back({ .position = { 1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, 1.0f } });
	primitiveData_.vertices.push_back({ .position = { -1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 0.0f }, .normal = { 0.0f, 0.0f, 1.0f } });
	primitiveData_.vertices.push_back({ .position = { 1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 1.0f }, .normal = { 0.0f, 0.0f, 1.0f } });
	primitiveData_.vertices.push_back({ .position = { -1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 1.0f }, .normal = { 0.0f, 0.0f, 1.0f } });
	// インデックスデータの設定
	primitiveData_.indices.push_back(0);
	primitiveData_.indices.push_back(1);
	primitiveData_.indices.push_back(2);
	primitiveData_.indices.push_back(2);
	primitiveData_.indices.push_back(1);
	primitiveData_.indices.push_back(3);

	primitiveData_.color = { 1.0f,1.0f,1.0f,1.0f };
	primitiveData_.uvMatrix = MakeIdentityMatrix4x4();
}

void Plane::Update() {
	BasePrimitiveShape3D::Update();
}
