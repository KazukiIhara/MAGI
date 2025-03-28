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

	// 指定したレンダーテクスチャを描画
	void Draw(const RenderTextureType& renderTextureType);
	// 指定したレンダーテクスチャを取得
	BaseRenderTexture* GetRenderTexture(const RenderTextureType& renderTextureType);
private:
	// 初期化処理
	void Initialize();
	// レンダーテクスチャを追加
	void AddRenderTexture(const RenderTextureType& renderTextureType, std::unique_ptr<BaseRenderTexture> renderTexture);
private:
	// レンダーテクスチャコンテナ
	std::map<RenderTextureType, std::unique_ptr<BaseRenderTexture>> renderTextures_;
};