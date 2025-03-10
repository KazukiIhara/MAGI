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

void RenderTextureManager::AddRenderTexture(const std::string& renderTextureName, std::unique_ptr<BaseRenderTexture> renderTexture) {
	renderTextures_.insert(std::pair(renderTextureName, std::move(renderTexture)));
}

BaseRenderTexture* RenderTextureManager::Find(const std::string& renderTextureName) {
	if (renderTextures_.contains(renderTextureName)) {
		return renderTextures_.at(renderTextureName).get();
	}
	return nullptr;
}

void RenderTextureManager::Initialize() {
	// Noneポストエフェクト
	std::unique_ptr<NonePostEffectRenderTexture> nonePostEffectRenderTexture = std::make_unique<NonePostEffectRenderTexture>();
	AddRenderTexture("NonePostEffect", std::move(nonePostEffectRenderTexture));

}