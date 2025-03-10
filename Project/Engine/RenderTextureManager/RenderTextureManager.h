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
private:
	void Initialize();
	void AddRenderTexture(const std::string& renderTextureName, std::unique_ptr<BaseRenderTexture> renderTexture);
private:
	// レンダーテクスチャコンテナ
	std::map<std::string, std::unique_ptr<BaseRenderTexture>> renderTextures_;
};