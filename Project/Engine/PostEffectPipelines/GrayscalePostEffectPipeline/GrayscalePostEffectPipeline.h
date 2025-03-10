#pragma once

#include "PostEffectPipelines/BaseSimplePostEffectPipeline/BaseSimplePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class GrayscalePostEffectPipeline:public BaseSimplePostEffectPipeline {
public:
	GrayscalePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~GrayscalePostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};