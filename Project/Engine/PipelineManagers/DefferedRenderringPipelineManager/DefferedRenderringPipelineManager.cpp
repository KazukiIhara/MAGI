#include "DefferedRenderringPipelineManager.h"

DefferedRenderringPipelineManager::DefferedRenderringPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
}

DefferedRenderringPipelineManager::~DefferedRenderringPipelineManager() {
}

void DefferedRenderringPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
}

ID3D12RootSignature* DefferedRenderringPipelineManager::GetRootSignature(DefferedRenderringType pipelineState) {
	return nullptr;
}

ID3D12PipelineState* DefferedRenderringPipelineManager::GetPipelineState(DefferedRenderringType pipelineState) {
	return nullptr;
}

void DefferedRenderringPipelineManager::SetRootSignature(DefferedRenderringType pipelineState) {
}

void DefferedRenderringPipelineManager::SetPipelineState(DefferedRenderringType pipelineState) {
}
