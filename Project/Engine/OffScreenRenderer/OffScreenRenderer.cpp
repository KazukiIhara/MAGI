#include "OffScreenRenderer.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/RenderTarget/RenderTarget.h"
#include "RenderTextureManager/RenderTextureManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"

OffScreenRenderer::OffScreenRenderer(DirectXCommand* directXCommand, RenderTarget* renderTarget, RenderTextureManager* renderTextureManager, PostEffectPipelineManager* postEffectPipelineManager) {
	SetDirectXCommand(directXCommand);
	SetRenderTarget(renderTarget);
	SetRenderTextureManager(renderTextureManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);
	Logger::Log("PostEffectSwitcher Initialize\n");
}

OffScreenRenderer::~OffScreenRenderer() {
	Logger::Log("PostEffectSwitcher Finalize\n");
}

void OffScreenRenderer::SetCurrentPostEffect(const PostEffectPipelineStateType& postEffectPipelineType) {
	currentPostEffect_ = postEffectPipelineType;
}

PostEffectPipelineStateType OffScreenRenderer::GetCurrentPostEffect() const {
	return currentPostEffect_;
}

void OffScreenRenderer::SetClearRenderTarget() {
	// レンダーテクスチャのタイプ
	RenderTargetType renderTargetType{};
	// ポストエフェクトに応じて最適なレンダーテクスチャを選択
	switch (currentPostEffect_) {
		case PostEffectPipelineStateType::None:
			renderTargetType = RenderTargetType::SimpleRenderTexture;
			break;
		case PostEffectPipelineStateType::Grayscale:
			renderTargetType = RenderTargetType::SimpleRenderTexture;
			break;
	}
	// レンダーターゲットを設定
	renderTarget_->SetRenderTarget(renderTargetType);
	// レンダーターゲットをクリア
	renderTarget_->ClearRenderTarget(renderTargetType);
}

void OffScreenRenderer::DrawCurrentRenderTexture() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(currentPostEffect_));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(currentPostEffect_, BlendMode::None));

	// ポストエフェクトに応じて最適なレンダーテクスチャを選択
	switch (currentPostEffect_) {
		case PostEffectPipelineStateType::None:
			currentRenderTextureType_ = RenderTextureType::Simple;
			break;
		case PostEffectPipelineStateType::Grayscale:
			currentRenderTextureType_ = RenderTextureType::Simple;
			break;
	}
	// レンダーテクスチャ描画
	renderTextureManager_->Draw(currentRenderTextureType_);
}

uint32_t OffScreenRenderer::GetCurrentRenderTextureSrvIndex() {
	return renderTextureManager_->GetRenderTexture(currentRenderTextureType_)->GetSrvIndex();
}

uint32_t OffScreenRenderer::GetForEngineUIRenderTexSrvIndex() {
	return 0;
}

void OffScreenRenderer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void OffScreenRenderer::SetRenderTarget(RenderTarget* renderTarget) {
	assert(renderTarget);
	renderTarget_ = renderTarget;
}

void OffScreenRenderer::SetRenderTextureManager(RenderTextureManager* renderTextureManager) {
	assert(renderTextureManager);
	renderTextureManager_ = renderTextureManager;
}

void OffScreenRenderer::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManger) {
	assert(postEffectPipelineManger);
	postEffectPipelineManager_ = postEffectPipelineManger;
}
