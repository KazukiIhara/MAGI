#pragma once

#include "ViewManagers/BaseViewManager/BaseViewManager.h"

class SRVUAVManager:public BaseViewManager {
public:
	SRVUAVManager(DXGI* dxgi);
	~SRVUAVManager()override;

	// ディスクリプタヒープの作成
	void CreateDescriptorHeap()override;
	// SRV生成(テクスチャ用)
	void CreateSrvTexture2d(uint32_t viewIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	// SRV生成(StructuredBuffer用)
	void CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// UAV作成
	void CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);

public: // 公開メンバ変数
	// 最大SRV数
	static const uint32_t kMaxViewCount_ = 512;
};