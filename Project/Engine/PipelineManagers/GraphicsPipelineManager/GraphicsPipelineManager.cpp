#include "GraphicsPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GraphicsPipelineManager::GraphicsPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("GraphicsPipelineManager Initialize");
}

GraphicsPipelineManager::~GraphicsPipelineManager() {
	Logger::Log("GraphicsPipelineManager Finalize");
}

void GraphicsPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {


}

ID3D12RootSignature* GraphicsPipelineManager::GetRootSignature(GraphicsPipelineStateType pipelineState) {
	return nullptr;
}

ID3D12PipelineState* GraphicsPipelineManager::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return nullptr;
}

void GraphicsPipelineManager::SetRootSignature(GraphicsPipelineStateType pipelineState) {}

void GraphicsPipelineManager::SetPipelineState(GraphicsPipelineStateType pipelineState) {}
