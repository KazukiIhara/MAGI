#include "Renderer3DManager.h"

#include <cassert>

#include "Logger/Logger.h"

Renderer3DManager::Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Initialize\n");
}

Renderer3DManager::~Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Finalize\n");
}

ModelRenderer* Renderer3DManager::Add(std::unique_ptr<ModelRenderer> modelRenderer) {
	assert(modelRenderer && "ModelRenderer must not be null");
	ModelRenderer* ptr = modelRenderer.get();
	modelRenderers_.push_back(std::move(modelRenderer));
	return ptr;
}

void Renderer3DManager::Draw() {
	for (auto& modelRenderer : modelRenderers_) {
		modelRenderer->Draw();
	}
}

void Renderer3DManager::DeleteGarbage() {
	for (auto& modelRenderer : modelRenderers_) {
		if (modelRenderer && !modelRenderer->GetisAlive()) {
			modelRenderer->Finalize();
		}
	}
	std::erase_if(modelRenderers_, [](const std::unique_ptr<ModelRenderer>& modelRenderer) {
		return modelRenderer && !modelRenderer->GetisAlive();
		});



}

void Renderer3DManager::Clear() {
	modelRenderers_.clear();


}