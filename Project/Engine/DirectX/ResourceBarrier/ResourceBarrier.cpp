#include "ResourceBarrier.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/SwapChain/SwapChain.h"

ResourceBarrier::ResourceBarrier(DirectXCommand* directXCommand, SwapChain* swapChain) {
	Initialize(directXCommand, swapChain);
	Logger::Log("ResourceBarrier Initialize");
}

ResourceBarrier::~ResourceBarrier() {
	Logger::Log("ResourceBarrier Finalize");
}

void ResourceBarrier::Initialize(DirectXCommand* directXCommand, SwapChain* swapChain) {
	// インスタンスをセット
	SetSwapChain(swapChain);
	SetDirectXCommand(directXCommand);
	// タイプはトランジション
	swapChainBarrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// None
	swapChainBarrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
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

void ResourceBarrier::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand_;
}

void ResourceBarrier::SetSwapChain(SwapChain* swapChain) {
	assert(swapChain);
	swapChian_ = swapChain;
}
