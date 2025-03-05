#include "PostEffectPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

PostEffectPipelineManager::PostEffectPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("PostEffectPipelineManager Initialize\n");
}

PostEffectPipelineManager::~PostEffectPipelineManager() {
	Logger::Log("PostEffectPipelineManager Finalize\n");
}

void PostEffectPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
}

ID3D12RootSignature* PostEffectPipelineManager::GetRootSignature(PostEffectPipelineStateType pipelineState) {
	return nullptr;
}

ID3D12PipelineState* PostEffectPipelineManager::GetPipelineState(PostEffectPipelineStateType pipelineState, BlendMode blendMode) {
	return nullptr;
}

void PostEffectPipelineManager::SetRootSignature(PostEffectPipelineStateType pipelineState) {
}

void PostEffectPipelineManager::SetPipelineState(PostEffectPipelineStateType pipelineState) {
}
