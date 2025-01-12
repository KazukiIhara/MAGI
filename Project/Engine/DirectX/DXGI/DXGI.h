#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

#include "ComPtr/ComPtr.h"

class DXGI {
public:
	// コンストラクタ
	DXGI();

	// 初期化
	void Initialize();

	// ディスクリプタヒープの作成
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	// バッファリソースの作成
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes, bool isforUAV = false);
	// デプスステンシルリソースの作成
	ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);

	// DXGIデバイスの取得
	ID3D12Device* GetDevice();
	// DXGIファクトリーの取得
	IDXGIFactory7* GetFactory();
private:
	// エラー判別
	HRESULT hr_ = S_FALSE;
	// DXGIファクトリー
	ComPtr <IDXGIFactory7> dxgiFactory_ = nullptr;
	// 使用するアダプタ用の変数
	ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;
	// Device
	ComPtr<ID3D12Device> device_ = nullptr;
};