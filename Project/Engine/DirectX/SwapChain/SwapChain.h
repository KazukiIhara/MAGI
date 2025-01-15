#pragma once

// C++
#include <cmath>
#include <cassert>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// 自作ファイル
#include "ComPtr/ComPtr.h"

// 前方宣言
class WindowApp;
class DXGI;
class DirectXCommand;
class RTVManager;

class SwapChain {
public:
	SwapChain(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, RTVManager* rtvManager);
	~SwapChain();
	// 初期化
	void Initialize(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, RTVManager* rtvManager);
	// GPUとOSに画面の交換を行うよう通知する
	void Present();
	// 現在のバックバッファのリソースを取得
	ID3D12Resource* GetCurrentBackBufferResource();
	// 描画先のRTVハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTVHandle();
private:
	// スワップチェーン作成
	void CreateSwapChain();
	// スワップチェーンのRTVリソースを作成
	void CreateResources();
	// RTVを作成
	void CreateRTV();
private:
	// WindowAppをセット
	void SetWindowApp(WindowApp* windowApp);
	// DXGIをセット
	void SetDXGI(DXGI* dxgi);
	// Commandをセット
	void SetCommand(DirectXCommand* command);
	// RTVManagerのセット
	void SetRTVManager(RTVManager* rtvManager);
private:
	// スワップチェーン
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	// スワップチェーンリソース
	ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// RTVリソースのインデックス
	uint32_t rtvIndex_[2]{};
	// バックバッファインデックス
	UINT backBufferIndex_ = 0;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
private:
	// WindowAppのインスタンスを受け取る箱
	WindowApp* windowApp_ = nullptr;
	// dxgiのインスタンスを受け取る箱
	DXGI* dxgi_ = nullptr;
	// Commandのインスタンスを受け取る箱
	DirectXCommand* command_ = nullptr;
	// RTVmanagerのインスタンスを受け取る箱
	RTVManager* rtvManager_ = nullptr;
};