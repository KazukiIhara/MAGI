#include "Triangle3DGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

Triangle3DGraphicsPipeline::Triangle3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	: BaseGraphicsPipeline(dxgi, shaderCompiler) {}

void Triangle3DGraphicsPipeline::CreateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = 0;
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> sigBlob;
	ComPtr<ID3DBlob> errorBlob;
	HRESULT hr = D3D12SerializeRootSignature(
		&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&sigBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = dxgi_->GetDevice()->CreateRootSignature(
		0, sigBlob->GetBufferPointer(), sigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void Triangle3DGraphicsPipeline::CompileShaders() {
	meshShaderBlob_ = nullptr;
	meshShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/TriangleMS/TriangleMS.MS.hlsl", L"ms_6_6");
	assert(meshShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/TriangleMS/TriangleMS.PS.hlsl", L"ps_6_6");
	assert(pixelShaderBlob_ != nullptr);
}

void Triangle3DGraphicsPipeline::CreateGraphicsPipelineObject() {
	assert(rootSignature_);
	assert(meshShaderBlob_);
	assert(pixelShaderBlob_);

	HRESULT hr;

	// 共通のシェーダーバイナリ
	D3D12_SHADER_BYTECODE meshShader = {
		meshShaderBlob_->GetBufferPointer(),
		meshShaderBlob_->GetBufferSize()
	};
	D3D12_SHADER_BYTECODE pixelShader = {
		pixelShaderBlob_->GetBufferPointer(),
		pixelShaderBlob_->GetBufferSize()
	};

	// 共通のフォーマット・トポロジなど
	D3D12_RASTERIZER_DESC rasterizerDesc = RasterizerStateSetting();
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = DepthStecilDescSetting();

	static const DXGI_FORMAT rtvFormats[1] = {
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	};
	D3D12_RT_FORMAT_ARRAY rtvFormatArray = {};
	rtvFormatArray.NumRenderTargets = 1;
	rtvFormatArray.RTFormats[0] = rtvFormats[0];

	DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		D3D12_BLEND_DESC blendDesc = BlendStateSetting(i);

		// サブオブジェクトの構築
		struct {
			D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type;
			void* data;
		} subobjects[8];

		size_t index = 0;
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE, rootSignature_.Get() };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS, &meshShader };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS, &pixelShader };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER, &rasterizerDesc };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND, &blendDesc };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL, &depthStencilDesc };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS, &rtvFormatArray };
		subobjects[index++] = { D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT, &dsvFormat };

		// ストリーム構築
		D3D12_PIPELINE_STATE_STREAM_DESC streamDesc{};
		streamDesc.SizeInBytes = sizeof(subobjects[0]) * index;
		streamDesc.pPipelineStateSubobjectStream = subobjects;

		// PSO作成
		pipelineState_[i] = nullptr;
		hr = dxgi_->GetDevice10()->CreatePipelineState(
			&streamDesc, IID_PPV_ARGS(&pipelineState_[i]));
		assert(SUCCEEDED(hr));
	}
}


D3D12_BLEND_DESC Triangle3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
	D3D12_BLEND_DESC blendDesc{};
	switch (blendModeNum) {
		case 0:// kBlendModeNone
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			break;
		case 1:// kBlendModeNormal
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 2:// kBlendModeAdd
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;
		case 3:// kBlendModeSubtract
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 4:// kBlendModeMultiply
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case 5:// kBlendModeScreen
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
			break;
	}
	// 全ての色要素を書き込む
	// ブレンドモードNone D3D12_COLOR_WRITE_ENABLE_ALLだけ

	return blendDesc;
}

D3D12_DEPTH_STENCIL_DESC Triangle3DGraphicsPipeline::DepthStecilDescSetting() {
	D3D12_DEPTH_STENCIL_DESC depthDesc{};
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	return depthDesc;
}

D3D12_INPUT_LAYOUT_DESC Triangle3DGraphicsPipeline::InputLayoutSetting() {
	return { nullptr, 0 }; // MeshShaderでは使用しない
}

D3D12_RASTERIZER_DESC Triangle3DGraphicsPipeline::RasterizerStateSetting() {
	D3D12_RASTERIZER_DESC desc{};
	desc.FillMode = D3D12_FILL_MODE_SOLID;
	desc.CullMode = D3D12_CULL_MODE_BACK;
	desc.FrontCounterClockwise = FALSE;
	return desc;
}