#pragma once

// C++
#include <string>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Includes/Enums/GraphicsPipelineEnum.h"

// 前方宣言
class DXGI;

class GraphicsPipelineManager {
public:
	GraphicsPipelineManager(DXGI* dxgi);
	~GraphicsPipelineManager();

	void Initialize(DXGI* dxgi);

	// ルートシグネイチャのゲッター
	ID3D12RootSignature* GetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインステイトのゲッター
	ID3D12PipelineState* GetPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

	// ルートシグネイチャをセット
	void SetRootSignature(GraphicsPipelineStateType pipelineState);

	// パイプラインをセット
	void SetPipelineState(GraphicsPipelineStateType pipelineState);

private: // 静的メンバ変数
	// パイプラインの種類の数 
	static const uint32_t kGraphicsPipelineStateNum = static_cast<uint32_t>(GraphicsPipelineStateType::Num);
	// ブレンドモードの種類の数
	static const uint32_t kBlendModeNum_ = static_cast<uint32_t>(BlendMode::Num);
private: // メンバ変数
	// ルートシグネイチャ
	ComPtr<ID3D12RootSignature> rootSignatures_[kGraphicsPipelineStateNum];
	// グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> graphicsPipelineStates_[kGraphicsPipelineStateNum][kBlendModeNum_];
private:
	// Object3DGraphicsPipeline
	std::unique_ptr<Object3DGraphicsPipeline> object3DGraphicsPipeline_ = nullptr;

};