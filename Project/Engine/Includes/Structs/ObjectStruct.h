#pragma once

// C++
#include <string>

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"
#include "Enums/Primitive3DEnum.h"

/// <summary>
/// UVトランスフォーム
/// </summary>
struct UVTransform {
	Vector2 scale = { 1.0f,1.0f };
	float rotateZ = 0.0f;
	Vector2 translate = { 0.0f,0.0f };
};

// オイラー角3DTrasform
struct EulerTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// クオータニオン角3DTransform
struct QuaternionTransform3D {
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Quaternion rotate = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 translate = { 0.0f,0.0f,0.0f };
};

// トランスフォーメーション構造体
struct TransformationMatrix {
	Matrix4x4 World;
	Matrix4x4 WorldInverseTransepose;
};

// 3D用マテリアル構造体
struct Material3D {
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	bool enableLighting;
	bool enableSpecularRef;
	UVTransform uvTransform;
	float shininess;
};

// 2D用マテリアル構造体
struct Material2D {
	Vector4 color;
	Matrix4x4 uvTransformMatrix;
};

// GPUに送る3D用マテリアル構造体
struct Material3DForGPU {
	Vector4 color;
	int32_t enableLighting;
	int32_t enableSpecularRef;
	float padding[2];
	Matrix4x4 uvTransformMatrix;
	float shininess;
};

/// <summary>
/// 3Dのラインデータ
/// </summary>
struct LineData3D {
	Vector3 start;
	Vector3 end;
	Vector4 color;
};

/// <summary>
/// 3Dの三角形データ
/// </summary>
struct TriangleData3D {
	Vector3 verticesOffsets[3]{
		{ -0.5f,-0.5f,0.0f },
		{ 0.0f,0.5f,0.0f },
		{ 0.5f,-0.5f,0.0f },
	};
};

/// <summary>
/// GPUに送る3Dの三角形データ
/// </summary>
struct TriangleData3DForGPU {
	Matrix4x4 worldMatrix;
	Vector4 Offsets[3];
};

/// <summary>
/// 3D板ポリの形状データ
/// </summary>
struct PlaneData3D {
	Vector3 verticesOffsets[4]{
		{ -0.5f,0.5f},
		{ 0.5f,0.5f},
		{ -0.5f,-0.5f},
		{ 0.5f,-0.5f},
	};
};

/// <summary>
/// GPUに送る3Dの板ポリデータ
/// </summary>
struct PlaneData3DForGPU {
	Matrix4x4 worldMatrix;
	Vector4 offsets[4];
};

/// <summary>
/// 3D球体の形状データ
/// </summary>
struct Sphere3DData {
	float radius;
	uint32_t verticalSegments;
	uint32_t horizontalSegments;
};

/// <summary>
/// GPUに送る3Dの球体データ
/// </summary>
struct SphereData3DForGPU {
	Matrix4x4 worldMatrix;
	Matrix4x4 worldInverseTranspose;
	float radius;
	uint32_t longitudeSegments;
	uint32_t latitudeSegments;
	float padding;
};

/// <summary>
/// Primitiveマテリアルデータ
/// </summary>
struct PrimitiveMaterialData3D {
	uint32_t textureIndex = 0xFFFFFFFF; // 無効値
	RGBA baseColor = Color::White;
	Vector2 uvTransform = { 0.0f,0.0f };
	Vector2 uvScale = { 1.0f,1.0f };
	float uvRotate = 0.0f;
};

/// <summary>
/// GPUに送る3DのPrimitiveマテリアルデータ
/// </summary>
struct PrimitiveMaterialData3DForGPU {
	uint32_t textureIndex;
	float padding0[3];
	Vector4 baseColor;
	Matrix4x4 uvMatrix;
};