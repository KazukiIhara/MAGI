#include "RenderTextureManager.h"

#include "Logger/Logger.h"

RenderTextureManager::RenderTextureManager() {
	Initialize();
	Logger::Log("RenderTexture Initialize\n");
}

RenderTextureManager::~RenderTextureManager() {
	Logger::Log("RenderTexture Finalize\n");
}

void RenderTextureManager::Draw(const RenderTextureType& renderTextureType) {
	GetRenderTexture(renderTextureType)->Draw();
}

BaseRenderTexture* RenderTextureManager::GetRenderTexture(const RenderTextureType& renderTextureType) {
	if (renderTextures_.contains(renderTextureType)) {
		return renderTextures_.at(renderTextureType).get();
	}
	return nullptr;
}

void RenderTextureManager::Initialize() {
	// 
	// エンジン標準搭載のレンダーテクスチャを挿入していく
	// 

	// Simple
	std::unique_ptr<BaseRenderTexture> nonePostEffectRenderTexture = std::make_unique<BaseRenderTexture>();
	nonePostEffectRenderTexture->Initialize();
	AddRenderTexture(RenderTextureType::Simple, std::move(nonePostEffectRenderTexture));

}

void RenderTextureManager::AddRenderTexture(const RenderTextureType& renderTextureType, std::unique_ptr<BaseRenderTexture> renderTexture) {
	renderTextures_.insert(std::pair(renderTextureType, std::move(renderTexture)));
}