#include "RenderPipelineController.h"

#include <cassert>

#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"

RenderPipelineController::RenderPipelineController(PostEffectPipelineManager* postEffectPipelineManager) {
	SetPostEffectPipelineManager(postEffectPipelineManager);

	// シーンを描画するレンダーテクスチャの初期化

}

RenderPipelineController::~RenderPipelineController() {

}

void RenderPipelineController::PreRender() {

}

void RenderPipelineController::ApplyPostEffect() {

}

void RenderPipelineController::FinalRender() {

}

void RenderPipelineController::EndFrame() {

}

void RenderPipelineController::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager) {
	assert(postEffectPipelineManager);
	postEffectPipelineManager_ = postEffectPipelineManager;
}
