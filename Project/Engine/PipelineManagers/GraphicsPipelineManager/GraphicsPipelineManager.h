#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "Includes/Enums/GraphicsPipelineEnum.h"

#include "GraphicsPipelines/Object2DGraphicsPipeline/Object2DGraphicsPipeline.h"
#include "GraphicsPipelines/Object3DGraphicsPipeline/Object3DGraphicsPipeline.h"
#include "GraphicsPipelines/Object3DGraphicsPipelineNormalMap/Object3DGraphicsPipelineNormalMap.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

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
	// Object3DGraphicsPipeline
	std::unique_ptr<Object3DGraphicsPipeline> object3DGraphicsPipeline_ = nullptr;
	// Object3DNormalMapGraphicsPipeline
	std::unique_ptr<Object3DGraphicsPipelineNormalMap> object3DNormalMapGraphicsPipeline_ = nullptr;
};