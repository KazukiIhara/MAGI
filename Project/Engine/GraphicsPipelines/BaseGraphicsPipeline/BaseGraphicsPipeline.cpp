#include "BaseGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

BaseGraphicsPipeline::BaseGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	SetDXGI(dxgi);
	SetShaderCompiler(shaderCompiler);
}

BaseGraphicsPipeline::~BaseGraphicsPipeline() {
}

void BaseGraphicsPipeline::Initialize() {
	CreateRootSignature();
	CompileShaders();
	CreateGraphicsPipelineObject();
}

ID3D12RootSignature* BaseGraphicsPipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* BaseGraphicsPipeline::GetPipelineState(BlendMode blendMode) {
	return pipelineState_[static_cast<uint32_t>(blendMode)].Get();
}

void BaseGraphicsPipeline::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void BaseGraphicsPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
	assert(shaderCompiler);
	shaderCompiler_ = shaderCompiler;
}
