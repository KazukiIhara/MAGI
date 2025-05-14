#pragma once



// C++
#include <cstdint>
#include <string>

#include "DirectX-Headers/include/directx/d3dx12_pipeline_state_stream.h"

// 数学ヘッダ
#include "Math/Types/Vector2.h"
#include "Math/Types/Vector4.h"
#include "Math/Types/Matrix4x4.h"

/// <summary>
/// スプライトメッシュシェーダー用のパイプラインストリーム
/// </summary>
struct SpritePipelineStateStream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE		rootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_MS					meshShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS					pixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER			rasterizer;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC			blend;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL			depthStencil;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtvFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT	dsvFormat;
};

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