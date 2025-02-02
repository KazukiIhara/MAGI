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
class ShaderCompiler;

/// <summary>
/// 3D描画パイプライン
/// </summary>
class Object3DGraphicsPipeline {
public:
	Object3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~Object3DGraphicsPipeline();

	// 初期化
	void Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler);
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
	// ShaderCompilerのインスタンスをセット
	void SetShaderCompiler(ShaderCompiler* shaderCompiler);

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプラインステート (ブレンドモードごとに設定)
	ComPtr<ID3D12PipelineState> pipelineState_[kBlendModeNum];
	// 頂点シェーダーのバイナリデータ
	ComPtr<ID3DBlob> vertexShaderBlob_;
	// ピクセルシェーダーのバイナリデータ
	ComPtr<ID3DBlob> pixelShaderBlob_;
private:
	DXGI* dxgi_ = nullptr;
	ShaderCompiler* shaderCompiler_ = nullptr;
};