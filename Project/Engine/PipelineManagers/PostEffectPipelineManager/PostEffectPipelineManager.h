#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// 前方宣言
class DXGI;
class ShaderCompiler;

// MyHedder
#include "Enums/PostEffectPipelineEnum.h"

#include "PostEffectPipelines/CopyPostEffectPipeline/CopyPostEffectPipeline.h"
#include "PostEffectPipelines/GrayscalePostEffectPipeline/GrayscalePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトパイプラインマネージャ
/// </summary>
class PostEffectPipelineManager {
public:
	PostEffectPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~PostEffectPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(PostEffectType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(PostEffectType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(PostEffectType pipelineState);

	// パイプラインをセット
	void SetPipelineState(PostEffectType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kPostEffectPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> postEffectPipelineStates_[kPostEffectPipelineStateNum][kBlendModeNum];
private:
	// NonePostEffectPipeline
	std::unique_ptr<CopyPostEffectPipeline> copyPostEffectPipeline_ = nullptr;
	// GrayscalePostEffectPipeline
	std::unique_ptr<GrayscalePostEffectPipeline> grayscalePostEffectPipeline_ = nullptr;
};