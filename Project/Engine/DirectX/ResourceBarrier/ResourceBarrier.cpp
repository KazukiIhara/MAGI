#include "ResourceBarrier.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/SwapChain/SwapChain.h"
#include "RenderTextureManager/RenderTextureManager.h"

ResourceBarrier::ResourceBarrier(DirectXCommand* directXCommand, SwapChain* swapChain, RenderTextureManager* renderTextureManager) {
	Initialize(directXCommand, swapChain, renderTextureManager);
	Logger::Log("ResourceBarrier Initialize\n");
}

ResourceBarrier::~ResourceBarrier() {
	Logger::Log("ResourceBarrier Finalize\n");
}

void ResourceBarrier::Initialize(DirectXCommand* directXCommand, SwapChain* swapChain, RenderTextureManager* renderTextureManager) {
	// インスタンスをセット
	SetSwapChain(swapChain);
	SetDirectXCommand(directXCommand);
	SetRenderTextureManager(renderTextureManager);
	// タイプはトランジション
	swapChainBarrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// None
	swapChainBarrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// レンダーテクスチャのポインタを取得
	simpleRenderTexture_ = renderTextureManager_->GetRenderTexture(RenderTextureType::Simple);
	// タイプはトランジション
	simpleRenderTextureBarrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// None
	simpleRenderTextureBarrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// RenderTexture用のリソースを受け取る
	simpleRenderTextureBarrier_.Transition.pResource = simpleRenderTexture_->GetResource();
}

void ResourceBarrier::PreDrawSwapChainResourceBarrierTransition() {
	// バリアを張る対象のリソース
	swapChainBarrier_.Transition.pResource = swapChian_->GetCurrentBackBufferResource();
	// 遷移前のリソースステート
	swapChainBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	// 遷移後のリソースステート
	swapChainBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// トランジションバリアを張る
	directXCommand_->GetList()->ResourceBarrier(1, &swapChainBarrier_);
}

void ResourceBarrier::PostDrawSwapChainResourceBarrierTransition() {
	// 遷移前のリソースステート
	swapChainBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 遷移後のリソースステート
	swapChainBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	// トランジションバリアを張る
	directXCommand_->GetList()->ResourceBarrier(1, &swapChainBarrier_);
}

void ResourceBarrier::PreDrawRenderTextureResourceBarrierTransition() {
	// 遷移前のリソースステート
	simpleRenderTextureBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 遷移後のリソースステート
	simpleRenderTextureBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// トランジションバリアを張る
	directXCommand_->GetList()->ResourceBarrier(1, &simpleRenderTextureBarrier_);
}

void ResourceBarrier::PostDrawRenderTextureResourceBarrierTransition() {
	// 遷移前のリソースステート
	simpleRenderTextureBarrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// 遷移後のリソースステート
	simpleRenderTextureBarrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// トランジションバリアを張る
	directXCommand_->GetList()->ResourceBarrier(1, &simpleRenderTextureBarrier_);
}

void ResourceBarrier::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void ResourceBarrier::SetSwapChain(SwapChain* swapChain) {
	assert(swapChain);
	swapChian_ = swapChain;
}

void ResourceBarrier::SetRenderTextureManager(RenderTextureManager* renderTextureManager) {
	assert(renderTextureManager);
	renderTextureManager_ = renderTextureManager;
}
