#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Includes/Enums/GraphicsPipelineEnum.h"

#include "GraphicsPipelines/Line3DGraphicsPipeline/Line3DGraphicsPipeline.h"
#include "GraphicsPipelines/Object2DGraphicsPipeline/Object2DGraphicsPipeline.h"
#include "GraphicsPipelines/Object3DGraphicsPipeline/Object3DGraphicsPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// 描画パイプラインマネージャ
/// </summary>
class GraphicsPipelineManager {
public:
	GraphicsPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GraphicsPipelineManager();

	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(GraphicsPipelineStateType pipelineState);

private: // メンバ変数
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kGraphicsPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kGraphicsPipelineStateNum][kBlendModeNum];
private:
	// Object2DGraphicsPipeline
	std::unique_ptr<Object2DGraphicsPipeline> object2DGraphicsPipeline_ = nullptr;
	// Line3DGrahicsPipelineS
	std::unique_ptr<Line3DGraphicsPipeline> line3DGraphicsPipeline_ = nullptr;
	// Object3DGraphicsPipeline
	std::unique_ptr<Object3DGraphicsPipeline> object3DGraphicsPipeline_ = nullptr;
};