#include "Sphere.h"
using namespace MAGIMath;

Sphere::Sphere(const std::string& textureFilePath)
	:BasePrimitiveShape3D() {
	BasePrimitiveShape3D::Initialize(textureFilePath);
}

void Sphere::CreateShape() {
	const float kLonEvery = std::numbers::pi_v<float> *2.0f / static_cast<float>(kSubdivision);
	const float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubdivision);

	// 頂点の生成
	for (uint32_t latIndex = 0; latIndex <= kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
		float sinLat = std::sinf(lat);
		float cosLat = std::cosf(lat);

		for (uint32_t lonIndex = 0; lonIndex <= kSubdivision; ++lonIndex) {
			float lon = kLonEvery * lonIndex;
			float sinLon = std::sinf(lon);
			float cosLon = std::cosf(lon);

			VertexData3D vertex{};
			vertex.position = { cosLat * cosLon, sinLat, cosLat * sinLon, 1.0f };
			vertex.normal = { cosLat * cosLon, sinLat, cosLat * sinLon };
			vertex.texcoord = { static_cast<float>(lonIndex) / static_cast<float>(kSubdivision),
								1.0f - static_cast<float>(latIndex) / static_cast<float>(kSubdivision) };
			vertex.tangent = { -sinLon, 0.0f, cosLon };

			primitiveData_.vertices.push_back(vertex);
		}
	}

	// インデックスの生成
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t first = latIndex * (kSubdivision + 1) + lonIndex;
			uint32_t second = first + kSubdivision + 1;

			// 三角形1
			primitiveData_.indices.push_back(first);
			primitiveData_.indices.push_back(second);
			primitiveData_.indices.push_back(first + 1);

			// 三角形2
			primitiveData_.indices.push_back(second);
			primitiveData_.indices.push_back(second + 1);
			primitiveData_.indices.push_back(first + 1);
		}
	}

	primitiveData_.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	primitiveData_.uvMatrix = MakeIdentityMatrix4x4();
}

void Sphere::Update() {
	BasePrimitiveShape3D::Update();
}
