#pragma once

#include "PostEffectPipelines/BaseNoParamaterPostEffectPipeline/BaseNoParamaterPostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class VignettePostEffectPipeline :public BaseNoParamaterPostEffectPipeline {
public:
	VignettePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~VignettePostEffectPipeline()override;

	void CreateRootSignature()override;

private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
