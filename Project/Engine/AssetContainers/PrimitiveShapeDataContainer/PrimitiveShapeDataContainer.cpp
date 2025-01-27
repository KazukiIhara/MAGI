#include "PrimitiveShapeDataContainer.h"

using namespace MAGIMath;

PrimitiveShapeDataContainer::PrimitiveShapeDataContainer() {
	Initialize();
}

PrimitiveShapeDataContainer::~PrimitiveShapeDataContainer() {
}

void PrimitiveShapeDataContainer::Initialize() {
	CreatePlane();
	CreateSphere();
}

void PrimitiveShapeDataContainer::CreatePlane() {
	PrimitiveData primitiveData{};
	// 頂点データの設定
	primitiveData.vertices.push_back({ .position = { 1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 0.0f }, .normal = { 0.0f, 0.0f, 1.0f },.tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = { -1.0f,  1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 0.0f }, .normal = { 0.0f, 0.0f, 1.0f } ,.tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = { 1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 0.0f, 1.0f }, .normal = { 0.0f, 0.0f, 1.0f },.tangent = { 1.0f, 0.0f, 0.0f } });
	primitiveData.vertices.push_back({ .position = { -1.0f, -1.0f, 0.0f, 1.0f }, .texcoord = { 1.0f, 1.0f }, .normal = { 0.0f, 0.0f, 1.0f },.tangent = { 1.0f, 0.0f, 0.0f } });
	// インデックスデータの設定
	primitiveData.indices.push_back(0);
	primitiveData.indices.push_back(1);
	primitiveData.indices.push_back(2);
	primitiveData.indices.push_back(2);
	primitiveData.indices.push_back(1);
	primitiveData.indices.push_back(3);

	primitiveData.color = { 1.0f,1.0f,1.0f,1.0f };
	primitiveData.uvMatrix = MakeIdentityMatrix4x4();

	// コンテナに挿入
	primitiveDatas_.insert(std::make_pair(Primitive3DType::Plane, primitiveData));
}

void PrimitiveShapeDataContainer::CreateSphere() {

}
