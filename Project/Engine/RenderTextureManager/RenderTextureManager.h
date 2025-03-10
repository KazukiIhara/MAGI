#pragma once

// C++
#include <map>
#include <string>
#include <memory>

// MyHedder
#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"

/// <summary>
/// レンダーテクスチャマネージャ
/// </summary>
class RenderTextureManager {
public:
	RenderTextureManager();
	~RenderTextureManager();

	void AddRenderTexture(const std::string& renderTextureName, std::unique_ptr<BaseRenderTexture> renderTexture);
	BaseRenderTexture* Find(const std::string& renderTextureName);
private:
	void Initialize();
private:
	// レンダーテクスチャコンテナ
	std::map<std::string, std::unique_ptr<BaseRenderTexture>> renderTextures_;
};