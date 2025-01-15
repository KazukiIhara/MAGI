#pragma once

#include <d3d12.h>

class DirectXCommand;
class SwapChain;

class ResourceBarrier {
public:
	ResourceBarrier(DirectXCommand* directXCommand, SwapChain* swapChain);
	~ResourceBarrier();

	// 初期化
	void Initialize(DirectXCommand* directXCommand, SwapChain* swapChain);

	// SwapChain用バリアの描画前設定
	void PreDrawSwapChainResourceBarrierTransition();
	// SwapChain用バリアの描画後設定
	void PostDrawSwapChainResourceBarrierTransition();



private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);
	// スワップチェーンのインスタンスをセット
	void SetSwapChain(SwapChain* swapChain);
private:
	// リソースバリア(スワップチェーン用)
	D3D12_RESOURCE_BARRIER swapChainBarrier_{};
	// リソースバリア(レンダーテクスチャ用)
	D3D12_RESOURCE_BARRIER renderTextureBarrier_{};
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChian_ = nullptr;
};