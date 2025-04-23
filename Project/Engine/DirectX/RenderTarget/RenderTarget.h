#pragma once

#include "Enums/RenderTargetEnumh.h"
#include "Structs/ColorStruct.h"

class DirectXCommand;
class SwapChain;
class DepthStencil;
class RTVManager;
class RenderTextureManager;

/// <summary>
/// レンダーターゲットを指定するクラス
/// </summary>
class RenderTarget {
public:
	RenderTarget(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil, RTVManager* rtvManager, RenderTextureManager* renderTextureManager);
	~RenderTarget();

	// 初期化
	void Initialize(DirectXCommand* command, SwapChain* swapChain, DepthStencil* depthStencil, RTVManager* rtvManager,RenderTextureManager* renderTextureManager);
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
	// RTVマネージャをセット
	void SetRTVManager(RTVManager* rtvManager);
	// レンダーテクスチャマネージャをセット
	void SetRenderTextureManager(RenderTextureManager* renderTextureManager);

private:
	// スワップチェーンのクリアカラー
	Vector4 clearColor_ = Color::Gray;
private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// スワップチェーンのインスタンスを受け取る箱
	SwapChain* swapChain_ = nullptr;
	// デプスステンシルのインスタンスを受け取る箱
	DepthStencil* depthStencil_ = nullptr;
	// RTVマネージャ
	RTVManager* rtvManager_ = nullptr;
	// レンダーテクスチャマネージャのインスタンスを受け取る箱
	RenderTextureManager* renderTextureManager_ = nullptr;
};