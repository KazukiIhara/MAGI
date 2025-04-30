#pragma once

#include "PostEffectPipelines/BaseWithParamaterPostEffectPipeline/BaseWithParamaterPostEffectPipeline.h"

/// <summary>
/// X軸ガウシアンブラー
/// </summary>
class GaussianBlurXPostEffectPipeline :public BaseWithParamaterPostEffectPipeline {
public:
	GaussianBlurXPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GaussianBlurXPostEffectPipeline()override;

private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};
