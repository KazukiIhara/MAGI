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
	// 3Dオブジェクトのグラフィックスパイプラインを生成、初期化
	object3DGraphicsPipeline_ = std::make_unique<Object3DGraphicsPipeline>(dxgi, shaderCompiler);
	SetRootSignature(GraphicsPipelineStateType::Object3D);
	SetPipelineState(GraphicsPipelineStateType::Object3D);

}

ID3D12RootSignature* GraphicsPipelineManager::GetRootSignature(GraphicsPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* GraphicsPipelineManager::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void GraphicsPipelineManager::SetRootSignature(GraphicsPipelineStateType pipelineState) {

}

void GraphicsPipelineManager::SetPipelineState(GraphicsPipelineStateType pipelineState) {

}
