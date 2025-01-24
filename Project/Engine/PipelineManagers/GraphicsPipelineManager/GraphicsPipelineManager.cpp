#include "GraphicsPipelineManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GraphicsPipelineManager::GraphicsPipelineManager(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	Initialize(dxgi, shaderCompiler);
	Logger::Log("GraphicsPipelineManager Initialize\n");
}

GraphicsPipelineManager::~GraphicsPipelineManager() {
	Logger::Log("GraphicsPipelineManager Finalize\n");
}

void GraphicsPipelineManager::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
	// 3Dオブジェクトのグラフィックスパイプラインを生成、初期化
	object3DGraphicsPipeline_ = std::make_unique<Object3DGraphicsPipeline>(dxgi, shaderCompiler);
	SetRootSignature(GraphicsPipelineStateType::Object3D);
	SetPipelineState(GraphicsPipelineStateType::Object3D);

	// 法線マップありのグラフィックスパイプラインを生成、初期化
	object3DNormalMapGraphicsPipeline_ = std::make_unique<Object3DGraphicsPipelineNormalMap>(dxgi, shaderCompiler);
	SetRootSignature(GraphicsPipelineStateType::Object3DNormalMap);
	SetPipelineState(GraphicsPipelineStateType::Object3DNormalMap);
}

ID3D12RootSignature* GraphicsPipelineManager::GetRootSignature(GraphicsPipelineStateType pipelineState) {
	return rootSignatures_[static_cast<uint32_t>(pipelineState)].Get();
}

ID3D12PipelineState* GraphicsPipelineManager::GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][static_cast<uint32_t>(blendMode)].Get();
}

void GraphicsPipelineManager::SetRootSignature(GraphicsPipelineStateType pipelineState) {
	// パイプラインステートごとに対応するルートシグネチャを設定
	switch (pipelineState) {
		case GraphicsPipelineStateType::Object3D:
			// 3Dオブジェクト描画用のルートシグネチャを設定
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = object3DGraphicsPipeline_->GetRootSignature();
			break;
		case GraphicsPipelineStateType::Object3DNormalMap:
			rootSignatures_[static_cast<uint32_t>(pipelineState)] = object3DNormalMapGraphicsPipeline_->GetRootSignature();
			break;

			// 他のパイプラインステートが追加された場合はここに追加
	}
}

void GraphicsPipelineManager::SetPipelineState(GraphicsPipelineStateType pipelineState) {
	switch (pipelineState) {
		case GraphicsPipelineStateType::Object3D:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = object3DGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;
		case GraphicsPipelineStateType::Object3DNormalMap:
			for (int mode = static_cast<uint32_t>(BlendMode::None); mode < static_cast<uint32_t>(BlendMode::Num); ++mode) {
				graphicsPipelineStates_[static_cast<uint32_t>(pipelineState)][mode] = object3DNormalMapGraphicsPipeline_->GetPipelineState(static_cast<BlendMode>(mode));
			}
			break;

			// 他のパイプラインステートが追加された場合はここに追加
	}
}
