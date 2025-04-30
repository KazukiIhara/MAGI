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
	// Copyのパイプラインを生成、初期化
	copyPostEffectPipeline_ = std::make_unique<CopyPostEffectPipeline>(dxgi, shaderCompiler);
	copyPostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Copy);
	SetPipelineState(PostEffectType::Copy);

	// Grayscaleのパイプラインを生成、初期化
	grayscalePostEffectPipeline_ = std::make_unique<GrayscalePostEffectPipeline>(dxgi, shaderCompiler);
	grayscalePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Grayscale);
	SetPipelineState(PostEffectType::Grayscale);

	// Vignetteのパイプラインを生成、初期化
	vignettePostEffectPipeline_ = std::make_unique<VignettePostEffectPipeline>(dxgi, shaderCompiler);
	vignettePostEffectPipeline_->Initialize();
	SetRootSignature(PostEffectType::Vignette);
	SetPipelineState(PostEffectType::Vignette);

}

ID3D12RootSignature* PostEffectPipelineManager::GetRootSignature(PostEffectType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* PostEffectPipelineManager::GetPipelineState(PostEffectType pipelineState, BlendMode blendMode) {
	return postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void PostEffectPipelineManager::SetRootSignature(PostEffectType pipelineState) {
	// パイプラインごとに対応するルートシグネイチャを設定
	switch (pipelineState) {
	case PostEffectType::Copy:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = copyPostEffectPipeline_->GetRootSignature();
		break;
	case PostEffectType::Grayscale:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = grayscalePostEffectPipeline_->GetRootSignature();
		break;
	case PostEffectType::Vignette:
		rootSignatures_[static_cast<uint32_t>(pipelineState)] = vignettePostEffectPipeline_->GetRootSignature();
		break;
	}
}

void PostEffectPipelineManager::SetPipelineState(PostEffectType pipelineState) {
	// パイプラインごとに対応するパイプラインステートを設定
	switch (pipelineState) {
	case PostEffectType::Copy:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = copyPostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case PostEffectType::Grayscale:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = grayscalePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	case PostEffectType::Vignette:
		for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
			postEffectPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = vignettePostEffectPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
		}
		break;
	}
}
