#include "GaussianBlurYPostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GaussianBlurYPostEffectPipeline::GaussianBlurYPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseWithParamaterPostEffectPipeline(dxgi, shaderCompiler) {
}

GaussianBlurYPostEffectPipeline::~GaussianBlurYPostEffectPipeline() {}

void GaussianBlurYPostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/GaussianBlur/GaussianBlur.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/GaussianBlur/GaussianBlurY.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}