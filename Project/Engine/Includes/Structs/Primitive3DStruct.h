#pragma once

#include "DirectX-Headers/include/directx/d3dx12_pipeline_state_stream.h"

/// <summary>
/// プリミティブメッシュシェーダー用のパイプラインストリーム
/// </summary>
struct Primitive3DPipelineStateStream {
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE		rootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_MS					meshShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS					pixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER			rasterizer;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC			blend;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL			depthStencil;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS rtvFormats;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT	dsvFormat;
};