#include "ShadowPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

ShadowPipelineManager::ShadowPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("ShadowPipelineManager Initialize\n");
}

ShadowPipelineManager::~ShadowPipelineManager() {
	Logger::Log("ShadowPipelineManager Finalize\n");
}

void ShadowPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	dxgi, shaderCompiler;
}

ID3D12RootSignature* ShadowPipelineManager::GetRootSignature(ShadowPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* ShadowPipelineManager::GetPipelineState(ShadowPipelineStateType pipelineState) {
	return graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)].Get();
}

void ShadowPipelineManager::SetRootSignature(ShadowPipelineStateType pipelineState) {
	pipelineState;
}

void ShadowPipelineManager::SetPipelineState(ShadowPipelineStateType pipelineState) {
	pipelineState;
}
