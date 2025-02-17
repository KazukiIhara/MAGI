#pragma once

#include "Enums/RenderTargetEnumh.h"
#include "Structs/ColorStruct.h"

class DirectXCommand;
class SwapChain;
class DepthStencil;

class RenderTarget {
public:
	RenderTarget(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil);
	~RenderTarget();

	// 初期化
	void Initialize(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil);
	// レンダーターゲットのセット
	void SetRenderTarget(const RenderTargetType& type);
	// レンダーターゲットのクリア
	void ClearRenderTarget(const RenderTargetType& type);

private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);
	// スワップチェーンのインスタンスをセット
	void SetSwapChain(SwapChain* swapChain);
	// デプスステンシルをセット
	void SetDepthStencil(DepthStencil* depthStencil);

private:
	// スワップチェーンのクリアカラー
	RGBA clearColor_ = Color::Gray;
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChain_ = nullptr;
	// デプスステンシルのインスタンスを受け取る箱
	DepthStencil* depthStencil_ = nullptr;

};