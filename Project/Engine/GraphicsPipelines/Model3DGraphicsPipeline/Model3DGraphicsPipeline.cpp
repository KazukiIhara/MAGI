#include "Model3DGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

Model3DGraphicsPipeline::Model3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	: BaseGraphicsPipeline(dxgi, shaderCompiler) {
}

void Model3DGraphicsPipeline::CreateRootSignature() {
	HRESULT hr;

	// Descriptor Ranges
	D3D12_DESCRIPTOR_RANGE descriptorRangeInstance{};
	descriptorRangeInstance.BaseShaderRegister = 0;
	descriptorRangeInstance.NumDescriptors = 1;
	descriptorRangeInstance.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeInstance.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeTextures{};
	descriptorRangeTextures.BaseShaderRegister = 1000;
	descriptorRangeTextures.NumDescriptors = 1024;
	descriptorRangeTextures.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeTextures.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeVertex{};
	descriptorRangeVertex.BaseShaderRegister = 1;
	descriptorRangeVertex.NumDescriptors = 1;
	descriptorRangeVertex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeVertex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeIndex{};
	descriptorRangeIndex.BaseShaderRegister = 2;
	descriptorRangeIndex.NumDescriptors = 1;
	descriptorRangeIndex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeIndex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeMeshlet{};
	descriptorRangeMeshlet.BaseShaderRegister = 3;
	descriptorRangeMeshlet.NumDescriptors = 1;
	descriptorRangeMeshlet.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeMeshlet.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[8] = {};

	// b0 : Camera
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[0].Descriptor.ShaderRegister = 0;

	// t0 : InstanceData
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRangeInstance;
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;

	// b1 : MaterialData
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParams[2].Descriptor.ShaderRegister = 1;

	// t1000 : Textures
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParams[3].DescriptorTable.pDescriptorRanges = &descriptorRangeTextures;
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;

	// RootConstants b2
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_AMPLIFICATION;
	rootParams[4].Constants.Num32BitValues = 4;
	rootParams[4].Constants.ShaderRegister = 2;

	// t1 : VertexBuffer
	rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[5].DescriptorTable.pDescriptorRanges = &descriptorRangeVertex;
	rootParams[5].DescriptorTable.NumDescriptorRanges = 1;

	// t2 : IndexBuffer
	rootParams[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_MESH;
	rootParams[6].DescriptorTable.pDescriptorRanges = &descriptorRangeIndex;
	rootParams[6].DescriptorTable.NumDescriptorRanges = 1;

	// t3 : MeshletBuffer
	rootParams[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_AMPLIFICATION;
	rootParams[7].DescriptorTable.pDescriptorRanges = &descriptorRangeMeshlet;
	rootParams[7].DescriptorTable.NumDescriptorRanges = 1;

	// Static Sampler
	D3D12_STATIC_SAMPLER_DESC staticSampler{};
	staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister = 0;
	staticSampler.RegisterSpace = 0;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// Root Signature
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = 1;
	rootSigDesc.pStaticSamplers = &staticSampler;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	ComPtr<ID3DBlob> sigBlob;
	ComPtr<ID3DBlob> errorBlob;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sigBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		assert(false);
	}

	hr = dxgi_->GetDevice()->CreateRootSignature(0, sigBlob->GetBufferPointer(), sigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));
}

void Model3DGraphicsPipeline::CompileShaders() {
	amplificationShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.AS.hlsl", L"as_6_5");
	assert(amplificationShaderBlob_);

	meshShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.MS.hlsl", L"ms_6_5");
	assert(meshShaderBlob_);

	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/Graphics/Model3D/Model3D.PS.hlsl", L"ps_6_5");
	assert(pixelShaderBlob_);
}

void Model3DGraphicsPipeline::CreateGraphicsPipelineObject() {
	assert(rootSignature_);
	assert(meshShaderBlob_);
	assert(pixelShaderBlob_);

	const D3D12_SHADER_BYTECODE meshShader = { meshShaderBlob_->GetBufferPointer(), meshShaderBlob_->GetBufferSize() };
	const D3D12_SHADER_BYTECODE pixelShader = { pixelShaderBlob_->GetBufferPointer(), pixelShaderBlob_->GetBufferSize() };
	const D3D12_SHADER_BYTECODE amplificationShader = { amplificationShaderBlob_->GetBufferPointer(), amplificationShaderBlob_->GetBufferSize() };

	const DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	const DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		const D3D12_RASTERIZER_DESC rasterizerDesc = RasterizerStateSetting();
		const D3D12_BLEND_DESC blendDesc = BlendStateSetting(i);
		const D3D12_DEPTH_STENCIL_DESC depthStencilDesc = DepthStecilDescSettingBlend(i);

		const CD3DX12_RASTERIZER_DESC rast(rasterizerDesc);
		const CD3DX12_BLEND_DESC blend(blendDesc);
		const CD3DX12_DEPTH_STENCIL_DESC ds(depthStencilDesc);

		D3D12_RT_FORMAT_ARRAY rtArray{};
		rtArray.NumRenderTargets = 1;
		rtArray.RTFormats[0] = rtvFormat;

		Primitive3DPipelineStateStream stream = {
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE(rootSignature_.Get()),
			CD3DX12_PIPELINE_STATE_STREAM_AS(amplificationShader),
			CD3DX12_PIPELINE_STATE_STREAM_MS(meshShader),
			CD3DX12_PIPELINE_STATE_STREAM_PS(pixelShader),
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER(rast),
			CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC(blend),
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL(ds),
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS(rtArray),
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT(dsvFormat)
		};

		D3D12_PIPELINE_STATE_STREAM_DESC streamDesc{};
		streamDesc.SizeInBytes = sizeof(stream);
		streamDesc.pPipelineStateSubobjectStream = &stream;

		HRESULT hr = dxgi_->GetDevice10()->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipelineState_[i]));
		if (FAILED(hr)) {
			Logger::Log("Model3DGraphicsPipeline: Failed to create PSO for blendMode " + std::to_string(i));
			assert(false);
		}
	}
}

D3D12_BLEND_DESC Model3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
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

D3D12_DEPTH_STENCIL_DESC Model3DGraphicsPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_DEPTH_STENCIL_DESC Model3DGraphicsPipeline::DepthStecilDescSettingBlend(uint32_t blendModeNum) {
	D3D12_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = TRUE;

	const auto mode = static_cast<BlendMode>(blendModeNum);
	switch (mode) {
	case BlendMode::Normal:
	case BlendMode::Add:
	case BlendMode::Subtract:
	case BlendMode::Multiply:
	case BlendMode::Screen:
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // ブレンド系は書き込まない
		break;
	default:
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		break;
	}

	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	return desc;
}

D3D12_INPUT_LAYOUT_DESC Model3DGraphicsPipeline::InputLayoutSetting() {
	return { nullptr, 0 };
}

D3D12_RASTERIZER_DESC Model3DGraphicsPipeline::RasterizerStateSetting() {
	D3D12_RASTERIZER_DESC desc{};
	desc.FillMode = D3D12_FILL_MODE_SOLID;
	desc.CullMode = D3D12_CULL_MODE_BACK;
	desc.FrontCounterClockwise = FALSE;
	return desc;
}
