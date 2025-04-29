#pragma once

#include "PostEffectPipelines/BaseSimplePostEffectPipeline/BaseSimplePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class CopyPostEffectPipeline:public BaseSimplePostEffectPipeline {
public:
	CopyPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~CopyPostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};