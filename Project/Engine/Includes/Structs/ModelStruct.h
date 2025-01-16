#pragma once

// C++
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"

/// <summary>
/// 3D頂点データ
/// </summary>
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

/// <summary>
/// マテリアルデータ
/// </summary>
struct MaterialData {
	std::string textureFilePath;
	Matrix4x4 uvMatrix;
	Vector4 color;
};

/// <summary>
/// メッシュデータ
/// </summary>
struct MeshData {
	std::vector<VertexData3D> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
};

/// <summary>
/// モデルデータ
/// </summary>
struct ModelData {
	std::vector<MeshData> meshes;
};

/// <summary>
/// GPUに送るマテリアルデータ
/// </summary>
struct MaterialForGPU {
	Vector4 color;
	Matrix4x4 uvMatrix;
};