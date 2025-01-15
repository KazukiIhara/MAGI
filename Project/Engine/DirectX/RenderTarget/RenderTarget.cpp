#include "RenderTarget.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"

RenderTarget::RenderTarget(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil) {
	Initialize(command, swapChain, depthStencil);
	Logger::Log("RenderTarget Initialize\n");
}

RenderTarget::~RenderTarget() {
	Logger::Log("RenderTarget Finalize\n");
}

void RenderTarget::Initialize(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil) {
	SetDirectXCommand(command);
	SetSwapChain(swapChain);
	SetDepthStencil(depthStencil);
}

void RenderTarget::SetRenderTarget(const RenderTargetType& type) {
	// RTVハンドルの受取先
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	switch (type) {
	case RenderTargetType::SwapChain:
		// SwapChainの現在のバックバッファのRTVハンドルを取得
		rtvHandle = swapChain_->GetCurrentBackBufferRTVHandle();
		break;

	}
	// デプスステンシルのDSVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvhandle = depthStencil_->GetDepthStencilResorceCPUHandle();
	// 描画先のRTVとDSVを指定
	directXCommand_->GetList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvhandle);
}

void RenderTarget::ClearRenderTarget(const RenderTargetType& type) {
	// RTVハンドルを受取先
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	// クリアカラー
	float clearColor[4]{};
	switch (type) {
	case RenderTargetType::SwapChain:
		// SwapChainの現在のバックバッファのRTVハンドルを取得
		rtvHandle = swapChain_->GetCurrentBackBufferRTVHandle();
		// クリアカラー変換
		clearColor[0] = clearColor_.r;
		clearColor[1] = clearColor_.g;
		clearColor[2] = clearColor_.b;
		clearColor[3] = clearColor_.a;
		break;

	}
	// 指定したレンダーターゲットと色で画面全体をクリアする
	directXCommand_->GetList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void RenderTarget::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RenderTarget::SetSwapChain(SwapChain* swapChain) {
	assert(swapChain);
	swapChain_ = swapChain;
}

void RenderTarget::SetDepthStencil(DepthStencil* depthStencil) {
	assert(depthStencil);
	depthStencil_ = depthStencil;
}
