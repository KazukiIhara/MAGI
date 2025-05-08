#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Enums/ShadowPipelineEnum.h"
#include "DirectX/ComPtr/ComPtr.h"

#include "ShadowPipelines/ModelShadowPipeline/ModelShadowPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// 影描画パイプラインマネージャ
/// </summary>
class ShadowPipelineManager {
public:
	ShadowPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~ShadowPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(ShadowPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(ShadowPipelineStateType pipelineState);

	// ルートシグネイチャをセット
	void SetRootSignature(ShadowPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(ShadowPipelineStateType pipelineState);

private:
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kShadowPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kShadowPipelineStateNum];
private:


};