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

#include "PostEffectPipelines/NonePostEffectPipeline/NonePostEffectPipeline.h"
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
	ID3D12RootSignature* GetRootSignature(PostEffectPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(PostEffectPipelineStateType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(PostEffectPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(PostEffectPipelineStateType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kPostEffectPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> postEffectPipelineStates_[kPostEffectPipelineStateNum][kBlendModeNum];
private:
	// NonePostEffectPipeline
	std::unique_ptr<NonePostEffectPipeline> nonePostEffectPipeline_ = nullptr;
	// GrayscalePostEffectPipeline
	std::unique_ptr<GrayscalePostEffectPipeline> grayscalePostEffectPipeline_ = nullptr;
};