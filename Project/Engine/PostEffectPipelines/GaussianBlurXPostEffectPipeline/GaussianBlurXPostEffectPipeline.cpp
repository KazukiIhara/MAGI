#include "GaussianBlurXPostEffectPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

GaussianBlurXPostEffectPipeline::GaussianBlurXPostEffectPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseWithParamaterPostEffectPipeline(dxgi, shaderCompiler) {
}

GaussianBlurXPostEffectPipeline::~GaussianBlurXPostEffectPipeline() {}

void GaussianBlurXPostEffectPipeline::CompileShaders() {
	vertexShaderBlob_ = nullptr;
	vertexShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/GaussianBlur/GaussianBlur.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = nullptr;
	pixelShaderBlob_ = shaderCompiler_->CompileShader(L"EngineAssets/Shaders/PostEffect/GaussianBlur/GaussianBlurX.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
}
