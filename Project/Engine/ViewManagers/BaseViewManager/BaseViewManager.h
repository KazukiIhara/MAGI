#pragma once

// C++
#include <cstdint>

// DirectX
#include <d3d12.h>

// ComPtr
#include "ComPtr/ComPtr.h"

// 前方宣言
class DXGI;

class BaseViewManager {
public:
	BaseViewManager(DXGI* dxgi);
	virtual ~BaseViewManager() = default;

	// 初期化
	void Initialize(DXGI* dxgi);
	// ディスクリプタヒープの作成
	virtual void CreateDescriptorHeap() {};
	// 割り当て関数
	uint32_t Allocate();
	// CPUの特定のインデックスハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandleCPU(uint32_t index);
	// GPUの特定のインデックスハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetDescriptorHandleGPU(uint32_t index);
	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	// ディスクリプタのサイズを取得
	uint32_t GetDescriptorSize() const;
private:
	// DXGIのセット
	void SetDXGI(DXGI* dxgi);
protected:
	// DescriptorHeap
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	// Discriptorのサイズ
	uint32_t descriptorSize_ = 0u;
	// 使用しているviewのインデックス
	uint32_t useIndex = 0;
protected:
	// dxgiのインスタンス
	DXGI* dxgi_ = nullptr;
};