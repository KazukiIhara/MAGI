#pragma once

// C++
#include <string>
#include <map>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "Structs/SkeletonStruct.h"
#include "Structs/SkinningStruct.h"

/// <summary>
/// 3D頂点データ
/// </summary>
struct VertexData3D {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
	Vector3 tangent;
};

/// <summary>
/// 2D頂点データ
/// </summary>
struct VertexData2D {
	Vector4 position;
	Vector2 texcoord;
};

/// <summary>
/// マテリアルデータ
/// </summary>
struct MaterialData {
	std::string textureFilePath;
	std::string normalMapTextureFilePath;
	Matrix4x4 uvMatrix;
	Vector4 color;
};

/// <summary>
/// ノード
/// </summary>
struct Node {
	QuaternionTransform3D transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
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
/// プリミティブ描画用データ
/// </summary>
struct PrimitiveData {
	std::vector<VertexData3D> vertices;
	std::vector<uint32_t> indices;
	Matrix4x4 uvMatrix;
	Vector4 color;
};

/// <summary>
/// モデルデータ
/// </summary>
struct ModelData {
	std::string name;
	std::vector<MeshData> meshes;
	Node rootNode;
	std::map<std::string, JointWeightData> skinClusterData;
};

/// <summary>
/// GPUに送るマテリアルデータ
/// </summary>
struct MaterialForGPU {
	Vector4 color;
	Matrix4x4 uvMatrix;
	int32_t enableNormalMap;
};