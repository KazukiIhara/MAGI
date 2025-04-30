#pragma once

#include "PostEffectPipelines/BaseSceneColorPostEffectPipeline/BaseSceneColorPostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class VignettePostEffectPipeline:public BaseSceneColorPostEffectPipeline {
public:
	VignettePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~VignettePostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
