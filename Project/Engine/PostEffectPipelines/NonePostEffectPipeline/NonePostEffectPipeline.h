#pragma once

#include "PostEffectPipelines/BaseSimplePostEffectPipeline/BaseSimplePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class NonePostEffectPipeline:public BaseSimplePostEffectPipeline {
public:
	NonePostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~NonePostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};