//#include "Line3DGraphicsPipeline.h"
//
//#include <cassert>
//
//#include "Logger/Logger.h"
//#include "DirectX/DXGI/DXGI.h"
//#include "DirectX/ShaderCompiler/ShaderCompiler.h"
//
//Line3DGraphicsPipeline::Line3DGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
//	Initialize(dxgi, shaderCompiler);
//	Logger::Log("Object3DGraphicsPipeline Initialize\n");
//}
//
//Line3DGraphicsPipeline::~Line3DGraphicsPipeline() {
//}
//
//void Line3DGraphicsPipeline::Initialize(DXGI* dxgi, ShaderCompiler* shaderCompiler) {
//	SetDXGI(dxgi);
//	SetShaderCompiler(shaderCompiler);
//	CreateRootSignature();
//	CompileShaders();
//	CreateGraphicsPipelineObject();
//}
//
//ID3D12RootSignature* Line3DGraphicsPipeline::GetRootSignature() {
//	return nullptr;
//}
//
//ID3D12PipelineState* Line3DGraphicsPipeline::GetPipelineState(BlendMode blendMode) {
//	return nullptr;
//}
//
//void Line3DGraphicsPipeline::CreateRootSignature() {
//}
//
//void Line3DGraphicsPipeline::CompileShaders() {
//}
//
//void Line3DGraphicsPipeline::CreateGraphicsPipelineObject() {
//}
//
//D3D12_BLEND_DESC Line3DGraphicsPipeline::BlendStateSetting(uint32_t blendModeNum) {
//	return D3D12_BLEND_DESC();
//}
//
//D3D12_DEPTH_STENCIL_DESC Line3DGraphicsPipeline::DepthStecilDescSetting() {
//	return D3D12_DEPTH_STENCIL_DESC();
//}
//
//D3D12_INPUT_LAYOUT_DESC Line3DGraphicsPipeline::InputLayoutSetting() {
//	return D3D12_INPUT_LAYOUT_DESC();
//}
//
//D3D12_RASTERIZER_DESC Line3DGraphicsPipeline::RasterizerStateSetting() {
//	return D3D12_RASTERIZER_DESC();
//}
//
//void Line3DGraphicsPipeline::SetDXGI(DXGI* dxgi) {
//}
//
//void Line3DGraphicsPipeline::SetShaderCompiler(ShaderCompiler* shaderCompiler) {
//}
