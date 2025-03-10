#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "Enums/RenderTextureType.h"
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

/// <summary>
/// レンダーテクスチャマネージャ
/// </summary>
class RenderTextureManager {
public:
	RenderTextureManager();
	~RenderTextureManager();

	// 指定されているレンダーテクスチャを描画
	void Draw(const RenderTextureType& renderTextureType);

	void AddRenderTexture(const RenderTextureType& renderTextureType, std::unique_ptr<BaseRenderTexture> renderTexture);
	BaseRenderTexture* GetRenderTexture(const RenderTextureType& renderTextureType);

private:
	// 初期化処理
	void Initialize();
private:
	// レンダーテクスチャコンテナ
	std::map<RenderTextureType, std::unique_ptr<BaseRenderTexture>> renderTextures_;
};