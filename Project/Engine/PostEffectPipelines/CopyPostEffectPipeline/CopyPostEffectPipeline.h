#pragma once

#include "PostEffectPipelines/BaseSimplePostEffectPipeline/BaseSimplePostEffectPipeline.h"

/// <summary>
/// ポストエフェクトなし
/// </summary>
class CopyPostEffectPipeline:public BaseSceneColorPostEffectPipeline {
public:
	CopyPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~CopyPostEffectPipeline()override;
private:
	// シェーダーをコンパイルする
	void CompileShaders()override;
};