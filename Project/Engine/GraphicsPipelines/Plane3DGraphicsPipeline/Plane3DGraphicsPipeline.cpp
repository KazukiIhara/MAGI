#include "Plane3DGraphicsPipeline.h"

#include <cassert>

#include "Logger/Logger.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

Plane3DGraphicsPipeline::Plane3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler)
	:BaseGraphicsPipeline(dxgi, shaderCompiler) {
}

Plane3DGraphicsPipeline::~Plane3DGraphicsPipeline() {
}

void Plane3DGraphicsPipeline::CreateRootSignature() {
}

void Plane3DGraphicsPipeline::CompileShaders() {
}

void Plane3DGraphicsPipeline::CreateGraphicsPipelineObject() {
}

D3D12_BLEND_DESC Plane3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
	return D3D12_BLEND_DESC();
}

D3D12_DEPTH_STENCIL_DESC Plane3DGraphicsPipeline::DepthStecilDescSetting() {
	return D3D12_DEPTH_STENCIL_DESC();
}

D3D12_INPUT_LAYOUT_DESC Plane3DGraphicsPipeline::InputLayoutSetting() {
	return D3D12_INPUT_LAYOUT_DESC();
}

D3D12_RASTERIZER_DESC Plane3DGraphicsPipeline::RasterizerStateSetting() {
	return D3D12_RASTERIZER_DESC();
}
