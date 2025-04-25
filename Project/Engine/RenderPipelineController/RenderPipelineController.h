#pragma once

#include <vector>
#include <memory>

#include "RenderTextures/ColorRenderTexture/ColorRenderTexture.h"
#include "Structs/PostEffectStruct.h"

// 前方宣言
class DirectXCommand;
class DepthStencil;
class Viewport;
class ScissorRect;
class PostEffectPipelineManager;

/// <summary>
/// パイプライン管理クラス
/// </summary>
class RenderController {
public:
	RenderController(
		DirectXCommand* directXCommand,
		DepthStencil* depthStencil,
		Viewport* viewport,
		ScissorRect* scissorRect,
		PostEffectPipelineManager* postEffectPipelineManager
	);
	~RenderController();

	// シーンを描画するための前準備
	void PreSceneRender();

	// シーン描画後の処理
	void PostSceneRender();

	// ポストエフェクトをpingPong方式でかけていく
	void ApplyPostEffect();

	// 最終描画用のテクスチャに描画
	void FinalRender();

	// フレーム終了処理
	void EndFrame();

	// 
	// 今フレームのポストエフェクトのコマンドを詰む関数を下に追加していく
	// 

private:
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetDepthStencil(DepthStencil* depthStencil);
	void SetViewport(Viewport* viewport);
	void SetScissorRect(ScissorRect* scissorRect);
	void SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager);

private:
	// 各インスタンスを受け取るクラス
	DirectXCommand* directXCommand_ = nullptr;
	DepthStencil* depthStencil_ = nullptr;
	Viewport* viewport_ = nullptr;
	ScissorRect* scissorRect_ = nullptr;
	PostEffectPipelineManager* postEffectPipelineManager_ = nullptr;
private:
	// 現在描画中のレンダーテクスチャのポインタ
	BaseRenderTexture* currentRenderTexture_ = nullptr;

	// シーンを描画するレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> sceneRenderTexture_ = nullptr;
	// 最終描画用のレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> finalRenderTexture_ = nullptr;

	// カラーポストエフェクト用のレンダーテクスチャ
	std::unique_ptr<ColorRenderTexture> colorPostEffectRenderTexture[2] = { nullptr,nullptr };
	// 現在使用中のカラーポストエフェクト用のレンダーテクスチャインデックス
	uint32_t currentColorPostEffectRenderTextureIndex_ = 0;

	// ポストエフェクトをかけるためのコマンド
	std::vector<PostEffectCommand> postEffectCommand_{};
};
