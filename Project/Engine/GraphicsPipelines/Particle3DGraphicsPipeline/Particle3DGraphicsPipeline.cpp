#include "Particle3DGraphicsPipeline.h"

Particle3DGraphicsPipeline::Particle3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseGraphicsPipeline(dxgi, shaderCompiler) {

}


Particle3DGraphicsPipeline::~Particle3DGraphicsPipeline() {}

void Particle3DGraphicsPipeline::CreateRootSignature() {}

void Particle3DGraphicsPipeline::CompileShaders() {}

void Particle3DGraphicsPipeline::CreateGraphicsPipelineObject() {}

D3D12_BLEND_DESC Particle3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
	return D3D12_BLEND_DESC();
}

D3D12_DEPTH_STENCIL_DESC Particle3DGraphicsPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_INPUT_LAYOUT_DESC Particle3DGraphicsPipeline::InputLayoutSetting() {
	return D3D12_INPUT_LAYOUT_DESC();
}

D3D12_RASTERIZER_DESC Particle3DGraphicsPipeline::RasterizerStateSetting() {
	return D3D12_RASTERIZER_DESC();
}
