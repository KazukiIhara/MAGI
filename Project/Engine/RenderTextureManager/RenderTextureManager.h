#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"
#include "RenderTextures/NonePostEffectRenderTexture/NonePostEffectRenderTexture.h"

/// <summary>
/// レンダーテクスチャマネージャ
/// </summary>
class RenderTextureManager {
public:
	RenderTextureManager();
	~RenderTextureManager();

	// 指定されているレンダーテクスチャを描画
	void DrawCurrentRenderTexture();

	void AddRenderTexture(const std::string& renderTextureName, std::unique_ptr<BaseRenderTexture> renderTexture);
	BaseRenderTexture* Find(const std::string& renderTextureName);

private:
	// 初期化処理
	void Initialize();
private:
	// 現在指定されているレンダーテクスチャ
	std::string currentRenderTextureName_ = "NonePostEffect";
	// レンダーテクスチャコンテナ
	std::map<std::string, std::unique_ptr<BaseRenderTexture>> renderTextures_;
};