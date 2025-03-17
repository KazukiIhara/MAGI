#pragma once

// MyHedder
#include "Enums/PostEffectPipelineEnum.h"

// 前方宣言
class DirectXCommand;
class RenderTarget;
class RenderTextureManager;
class PostEffectPipelineManager;

/// <summary>
/// パイプラインやレンダーターゲットを切り替えてオフスクリーンレンダリングをするクラス
/// </summary>
class OffScreenRenderer {
public:
	OffScreenRenderer(DirectXCommand* directXCommand, RenderTarget* renderTarget, RenderTextureManager* renderTextureManager, PostEffectPipelineManager* postEffectPipelineManager);
	~OffScreenRenderer();

	// ポストエフェクトを設定
	void SetCurrentPostEffect(const PostEffectPipelineStateType& postEffectPipelineType);
	// ポストエフェクトを取得
	PostEffectPipelineStateType GetCurrentPostEffect() const;

	// レンダーターゲットをセット
	void SetClearRenderTarget();
	// ポストエフェクトをかけたレンダーテクスチャを描画
	void DrawCurrentRenderTexture();

	uint32_t GetCurrentRenderTextureSrvIndex();

private:
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetRenderTarget(RenderTarget* renderTarget);
	void SetRenderTextureManager(RenderTextureManager* renderTextureManager);
	void SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManger);
private:
	// 現在のポストエフェクト
	PostEffectPipelineStateType currentPostEffect_ = PostEffectPipelineStateType::None;
	RenderTextureType currentRenderTextureType_{};
private:
	DirectXCommand* directXCommand_ = nullptr;
	RenderTarget* renderTarget_ = nullptr;
	RenderTextureManager* renderTextureManager_ = nullptr;
	PostEffectPipelineManager* postEffectPipelineManager_ = nullptr;
};