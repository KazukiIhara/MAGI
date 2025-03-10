#include "PostEffectSwitcher.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/RenderTarget/RenderTarget.h"
#include "RenderTextureManager/RenderTextureManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"

PostEffectSwitcher::PostEffectSwitcher(DirectXCommand* directXCommand, RenderTarget* renderTarget, RenderTextureManager* renderTextureManager, PostEffectPipelineManager* postEffectPipelineManager) {
	SetDirectXCommand(directXCommand);
	SetRenderTarget(renderTarget);
	SetRenderTextureManager(renderTextureManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);
	Logger::Log("PostEffectSwitcher Initialize\n");
}

PostEffectSwitcher::~PostEffectSwitcher() {
	Logger::Log("PostEffectSwitcher Finalize\n");
}

void PostEffectSwitcher::SetCurrentPostEffect(const PostEffectPipelineStateType& postEffectPipelineType) {
	currentPostEffect_ = postEffectPipelineType;
}

PostEffectPipelineStateType PostEffectSwitcher::GetCurrentPostEffect() const {
	return currentPostEffect_;
}

void PostEffectSwitcher::SetClearRenderTarget() {
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

void PostEffectSwitcher::DrawCurrentRenderTexture() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(currentPostEffect_));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(currentPostEffect_, BlendMode::None));

	// レンダーテクスチャのタイプ
	RenderTextureType renderTextureType{};

	// ポストエフェクトに応じて最適なレンダーテクスチャを選択
	switch (currentPostEffect_) {
		case PostEffectPipelineStateType::None:
			renderTextureType = RenderTextureType::Simple;
			break;
		case PostEffectPipelineStateType::Grayscale:
			renderTextureType = RenderTextureType::Simple;
			break;
	}
	// レンダーテクスチャ描画
	renderTextureManager_->Draw(renderTextureType);
}

void PostEffectSwitcher::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void PostEffectSwitcher::SetRenderTarget(RenderTarget* renderTarget) {
	assert(renderTarget);
	renderTarget_ = renderTarget;
}

void PostEffectSwitcher::SetRenderTextureManager(RenderTextureManager* renderTextureManager) {
	assert(renderTextureManager);
	renderTextureManager_ = renderTextureManager;
}

void PostEffectSwitcher::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManger) {
	assert(postEffectPipelineManger);
	postEffectPipelineManager_ = postEffectPipelineManger;
}
