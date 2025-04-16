#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

#include "DirectXHeaders/d3dx12_pipeline_state_stream.h"

struct Triangle3DPipelineStateStream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE     rootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_MS                 meshShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS                 pixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER         rasterizer;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC         blend;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL      depthStencil;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtvFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT dsvFormat;
};

/// <summary>
/// Mesh Shaderを使った単純な三角形描画用グラフィックスパイプライン
/// </summary>
class Triangle3DGraphicsPipeline: public BaseGraphicsPipeline {
public:
	Triangle3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Triangle3DGraphicsPipeline() override = default;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;

};
