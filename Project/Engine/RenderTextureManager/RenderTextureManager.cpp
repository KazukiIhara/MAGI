#include "RenderTextureManager.h"

#include "Logger/Logger.h"

RenderTextureManager::RenderTextureManager() {
	Logger::Log("RenderTexture Initialize\n");
}

RenderTextureManager::~RenderTextureManager() {
	Logger::Log("RenderTexture Finalize\n");
}