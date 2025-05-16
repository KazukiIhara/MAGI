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
	// Cylinderのシャドウパイプラインを生成、初期化
	cylinderShadowPipeline_ = std::make_unique<CylinderShadowPipeline>(dxgi, shaderCompiler);
	cylinderShadowPipeline_->Initialize();
	SetRootSignature(ShadowPipelineStateType::Cylinder);
	SetPipelineState(ShadowPipelineStateType::Cylinder);

	// Modelのシャドウパイプラインを生成、初期化
	modelShadowPipeline_ = std::make_unique<ModelShadowPipeline>(dxgi, shaderCompiler);
	modelShadowPipeline_->Initialize();
	SetRootSignature(ShadowPipelineStateType::Model);
	SetPipelineState(ShadowPipelineStateType::Model);
}

ID3D12RootSignature* ShadowPipelineManager::GetRootSignature(ShadowPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* ShadowPipelineManager::GetPipelineState(ShadowPipelineStateType pipelineState) {
	return shadowPipelineStates_[static_cast<uint32_t>(pipelineState)].Get();
}

void ShadowPipelineManager::SetRootSignature(ShadowPipelineStateType pipelineState) {
	switch (pipelineState) {
	case ShadowPipelineStateType::Model:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = modelShadowPipeline_->GetRootSignature();
		break;
	}
}

void ShadowPipelineManager::SetPipelineState(ShadowPipelineStateType pipelineState) {
	switch (pipelineState) {
	case ShadowPipelineStateType::Model:
		shadowPipelineStates_[static_cast<uint32_t>(pipelineState)] = modelShadowPipeline_->GetPipelineState();
		break;
	}
}
