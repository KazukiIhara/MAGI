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
	// Noneのパイプラインを生成、初期化
	nonePostEffectPipeline_ = std::make_unique<NonePostEffectPipeline>(dxgi, shaderCompiler);
	nonePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectPipelineStateType::None);
	SetPipelineState(PostEffectPipelineStateType::None);

	// Grayscaleのパイプラインを生成、初期化
	grayscalePostEffectPipeline_ = std::make_unique<GrayscalePostEffectPipeline>(dxgi, shaderCompiler);
	grayscalePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectPipelineStateType::Grayscale);
	SetPipelineState(PostEffectPipelineStateType::Grayscale);

}

ID3D12RootSignature* PostEffectPipelineManager::GetRootSignature(PostEffectPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* PostEffectPipelineManager::GetPipelineState(PostEffectPipelineStateType pipelineState, BlendMode blendMode) {
	return postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void PostEffectPipelineManager::SetRootSignature(PostEffectPipelineStateType pipelineState) {
	// パイプラインごとに対応するルートシグネイチャを設定
	switch (pipelineState) {
		case PostEffectPipelineStateType::None:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = nonePostEffectPipeline_->GetRootSignature();
			break;
		case PostEffectPipelineStateType::Grayscale:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = grayscalePostEffectPipeline_->GetRootSignature();
			break;
	}
}

void PostEffectPipelineManager::SetPipelineState(PostEffectPipelineStateType pipelineState) {
	// パイプラインごとに対応するパイプラインステートを設定
	switch (pipelineState) {
		case PostEffectPipelineStateType::None:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = nonePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case PostEffectPipelineStateType::Grayscale:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = grayscalePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
	}
}
