#pragma once

#include <d3d12.h>

class DirectXCommand;
class SwapChain;
class RenderTextureManager;
class BaseRenderTexture;

class ResourceBarrier {
public:
	ResourceBarrier(DirectXCommand* directXCommand, SwapChain* swapChain, RenderTextureManager* renderTextureManager);
	~ResourceBarrier();

	// 初期化
	void Initialize(DirectXCommand* directXCommand, SwapChain* swapChain, RenderTextureManager* renderTextureManager);

	// SwapChain用バリアの描画前設定
	void PreDrawSwapChainResourceBarrierTransition();
	// SwapChain用バリアの描画後設定
	void PostDrawSwapChainResourceBarrierTransition();

	// RenderTexture用バリアの描画前設定
	void PreDrawRenderTextureResourceBarrierTransition();
	// RenderTexture用のバリアの描画後設定
	void PostDrawRenderTextureResourceBarrierTransition();

private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);
	// スワップチェーンのインスタンスをセット
	void SetSwapChain(SwapChain* swapChain);
	// レンダーテクスチャマネージャのインスタンスをセット
	void SetRenderTextureManager(RenderTextureManager* renderTextureManager);
private:
	// リソースバリア(スワップチェーン用)
	D3D12_RESOURCE_BARRIER swapChainBarrier_{};
	// リソースバリア(レンダーテクスチャ用)
	D3D12_RESOURCE_BARRIER renderTextureBarrier_{};
	// レンダーテクスチャを受け取る
	BaseRenderTexture* renderTexture_ = nullptr;
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChian_ = nullptr;
	// レンダーテクスチャマネージャのインスタンスを受け取る箱
	RenderTextureManager* renderTextureManager_ = nullptr;
};