#include "BaseViewManager.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"

BaseViewManager::BaseViewManager(DXGI* dxgi) {
	Initialize(dxgi);
}

void BaseViewManager::Initialize(DXGI* dxgi) {
	// DXGIのセット
	SetDXGI(dxgi);
}

uint32_t BaseViewManager::Allocate() {
	uint32_t index = useIndex;
	useIndex++;
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE BaseViewManager::GetDescriptorHandleCPU(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE BaseViewManager::GetDescriptorHandleGPU(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

ID3D12DescriptorHeap* BaseViewManager::GetDescriptorHeap() const {
	return descriptorHeap_.Get();
}

uint32_t BaseViewManager::GetDescriptorSize() const {
	return descriptorSize_;
}

void BaseViewManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}
