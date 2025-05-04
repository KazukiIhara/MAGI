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
/// ディファードレンダリングパイプラインの基底クラス
/// </summary>
class BaseDefferedRenderringPipeline {
public:
	BaseDefferedRenderringPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	virtual ~BaseDefferedRenderringPipeline();

	// 初期化
	void Initialize();
	// ルートシグネチャを取得する
	ID3D12RootSignature* GetRootSignature();
	// 指定されたブレンドモードに対応するパイプラインステートを取得する
	ID3D12PipelineState* GetPipelineState(BlendMode blendMode);


private:

};