#pragma once

#include <vector>
#include <memory>

#include "RenderTextures/ColorRenderTexture/ColorRenderTexture.h"
#include "Structs/PostEffectStruct.h"

// 前方宣言
class PostEffectPipelineManager;

/// <summary>
/// パイプライン管理クラス
/// </summary>
class RenderPipelineController {
public:
	RenderPipelineController(PostEffectPipelineManager* postEffectPipelineManager);
	~RenderPipelineController();

	// シーンを描画するための前準備
	void PreRender();

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
	void SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager);

private:
	// ポストエフェクトパイプラインマネージャを受け取るクラス
	PostEffectPipelineManager* postEffectPipelineManager_ = nullptr;
private:
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
