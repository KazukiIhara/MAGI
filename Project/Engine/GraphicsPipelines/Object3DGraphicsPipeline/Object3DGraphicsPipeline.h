#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Includes/Enums/BlendModeEnum.h"

// 前方宣言
class DXGI;

class Object3DGraphicsPipeline {
public:
	Object3DGraphicsPipeline(DXGI* dxgi);
	~Object3DGraphicsPipeline();

	// 初期化
	void Initialize(DXGI* dxgi);
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState(BlendMode blendMode);

private:
	// ルートシグネチャを作成する
	void CreateRootSignature();

	// シェーダーをコンパイルする
	void CompileShaders();

	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject();

	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum);

	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting();

	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting();

	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting();
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;

	// グラフィックスパイプラインステート (ブレンドモードごとに設定)
	ComPtr<ID3D12PipelineState> pipelineState_[kBlendModeNum_];

	// 頂点シェーダーのバイナリデータ
	ComPtr<ID3DBlob> vertexShaderBlob_;

	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_;

private: // ポインタでDirectXManagerのインスタンスを保持
	DXGI* dxgi_ = nullptr;

};