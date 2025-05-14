#pragma once

// C++
#include <cstdint>
#include <string>

// 数学ヘッダ
#include "Math/Types/Vector2.h"
#include "Math/Types/Vector4.h"
#include "Math/Types/Matrix4x4.h"

/// <summary>
/// CPU側で使うスプライトのデータ
/// </summary>
struct SpriteData {
	Vector2 screenPosition;
	Vector2 size;
};

/// <summary>
/// CPU側で使うスプライトのマテリアルデータ
/// </summary>
struct SpriteMaterialData {
	uint32_t textureIndex;
	Vector4 color;
	Vector2 anchorPoint;
	bool isFlipX;
	bool isFlipY;
	Vector2 uvScale;
	float uvRotate;
	Vector2 uvTranslate;
};

/// <summary>
/// GPUに送るSpriteのデータ
/// </summary>
struct SpriteDataForGPU {
	// 
	// 座標や形状
	// 
	Matrix4x4 worldMatrix;
	Vector2 position[4];
	Vector2 texcooed[4];

	// 
	// マテリアル
	// 
	uint32_t textureIndex;
	float padding0[3];
	Vector4 baseColor;
	Matrix4x4 uvMatrix;
};