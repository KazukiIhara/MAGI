#include "RenderPipelineController.h"

#include <cassert>

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"


RenderPipelineController::RenderPipelineController(DirectXCommand* directXCommand, DepthStencil* depthStencil, Viewport* viewport, ScissorRect* scissorRect, PostEffectPipelineManager* postEffectPipelineManager) {
	SetDirectXCommand(directXCommand);
	SetDepthStencil(depthStencil);
	SetViewport(viewport);
	SetScissorRect(scissorRect);
	SetPostEffectPipelineManager(postEffectPipelineManager);


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

void RenderPipelineController::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RenderPipelineController::SetDepthStencil(DepthStencil* depthStencil) {
	assert(depthStencil);
	depthStencil_ = depthStencil;
}

void RenderPipelineController::SetViewport(Viewport* viewport) {
	assert(viewport);
	viewport_ = viewport;
}

void RenderPipelineController::SetScissorRect(ScissorRect* scissorRect) {
	assert(scissorRect);
	scissorRect_ = scissorRect;
}

void RenderPipelineController::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager) {
	assert(postEffectPipelineManager);
	postEffectPipelineManager_ = postEffectPipelineManager;
}
