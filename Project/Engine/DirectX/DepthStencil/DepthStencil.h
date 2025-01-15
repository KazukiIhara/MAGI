#pragma once
#include <cstdint>

#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"

class DXGI;
class DirectXCommand;
class DSVManager;

class DepthStencil {
public:
	DepthStencil(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager);
	~DepthStencil();

	// 初期化
	void Initialize(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager);
	// デプスをクリア
	void ClearDepthView();
	// デプスステンシルリソースのCPUハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilResorceCPUHandle();
private:
	// デプスステンシルリソースの作成
	void CreateDepthStencilView();
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// コマンドのインスタンスをセット
	void SetCommand(DirectXCommand* command);
	// DSVマネージャのインスタンスをセット
	void SetDSVManager(DSVManager* dsvManager);
private:
	// デプスステンシルリソース
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	// デプスステンシルリソースのインデックス
	uint32_t dsvIndex_ = 0;
private:
	// DXGIのインスタンスを受け取る箱
	DXGI* dxgi_ = nullptr;
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
	// DSVマネージャのインスタンスを受け取る箱
	DSVManager* dsvManager_ = nullptr;
};