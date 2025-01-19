#pragma once

#include "ViewManagers/BaseViewManager/BaseViewManager.h"

class DSVManager:public BaseViewManager {
public:
	DSVManager(DXGI* dxgi);
	~DSVManager()override;

	// ディスクリプタヒープの作成
	void CreateDescriptorHeap()override;
	// Texture2d用のDSVを作成
	void CreateDSVTexture2d(uint32_t dsvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
private:
	// 最大dsv数
	const uint32_t kMaxDSVCount_ = 1;
};