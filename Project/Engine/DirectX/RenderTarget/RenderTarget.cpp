#include "RenderTarget.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "RenderTextureManager/RenderTextureManager.h"

RenderTarget::RenderTarget(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil, RenderTextureManager* renderTextureManager) {
	Initialize(command, swapChain, depthStencil, renderTextureManager);
	Logger::Log("RenderTarget Initialize\n");
}

RenderTarget::~RenderTarget() {
	Logger::Log("RenderTarget Finalize\n");
}

void RenderTarget::Initialize(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil, RenderTextureManager* renderTextureManager) {
	SetDirectXCommand(command);
	SetSwapChain(swapChain);
	SetDepthStencil(depthStencil);
	SetRenderTextureManager(renderTextureManager);
}

void RenderTarget::SetRenderTarget(const RenderTargetType& type) {
	// RTVハンドルの受取先
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{};
	switch (type) {
		case RenderTargetType::SwapChain:
			// SwapChainの現在のバックバッファのRTVハンドルを取得
			rtvHandle = swapChain_->GetCurrentBackBufferRTVHandle();
			break;
		case RenderTargetType::SimpleRenderTexture:
			// RenderTextureのRTVハンドルを取得
			BaseRenderTexture* renderTexture = renderTextureManager_->GetRenderTexture(RenderTextureType::Simple);
			rtvHandle = renderTexture->GetRTVHandle();
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
		case RenderTargetType::SimpleRenderTexture:
			// RenderTextureのRTVハンドルを取得
			BaseRenderTexture* renderTexture = renderTextureManager_->GetRenderTexture(RenderTextureType::Simple);
			rtvHandle = renderTexture->GetRTVHandle();
			// RenderTextureのクリアカラーを取得
			Vector4 renderTextureClearColor = renderTexture->GetClearColor();
			// クリアカラー変換
			clearColor[0] = renderTextureClearColor.x;
			clearColor[1] = renderTextureClearColor.y;
			clearColor[2] = renderTextureClearColor.z;
			clearColor[3] = renderTextureClearColor.w;
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

void RenderTarget::SetRenderTextureManager(RenderTextureManager* renderTextureManager) {
	assert(renderTextureManager);
	renderTextureManager_ = renderTextureManager;
}
