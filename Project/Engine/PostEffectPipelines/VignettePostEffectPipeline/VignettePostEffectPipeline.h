#pragma once

#include "PostEffectPipelines/BaseSimplePostEffectPipeline/BaseSimplePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class VignettePostEffectPipeline:public BaseSimplePostEffectPipeline {
public:
	VignettePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~VignettePostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
