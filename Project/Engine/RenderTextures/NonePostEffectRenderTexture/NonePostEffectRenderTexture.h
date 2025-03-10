#pragma once

// MyHedder
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

/// <summary>
/// ポストエフェクトなしのレンダーテクスチャ
/// </summary>
class NonePostEffectRenderTexture :public BaseRenderTexture {
public:
	NonePostEffectRenderTexture();
	~NonePostEffectRenderTexture()override;
};