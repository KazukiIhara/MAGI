#include "RenderTextureManager.h"

#include "Logger/Logger.h"

#include "RenderTextures/NonePostEffectRenderTexture/NonePostEffectRenderTexture.h"

RenderTextureManager::RenderTextureManager() {
	Initialize();
	Logger::Log("RenderTexture Initialize\n");
}

RenderTextureManager::~RenderTextureManager() {
	Logger::Log("RenderTexture Finalize\n");
}

void RenderTextureManager::Initialize() {
	// Noneポストエフェクト
	std::unique_ptr<NonePostEffectRenderTexture> nonePostEffectRenderTexture = std::make_unique<NonePostEffectRenderTexture>();
	AddRenderTexture("NonePostEffect", std::move(nonePostEffectRenderTexture));


}

void RenderTextureManager::AddRenderTexture(const std::string& renderTextureName, std::unique_ptr<BaseRenderTexture> renderTexture) {
	renderTextures_.insert(std::pair(renderTextureName, std::move(renderTexture)));
}
