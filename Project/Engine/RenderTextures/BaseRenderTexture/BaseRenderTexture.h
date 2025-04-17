#pragma once

// C++
#include <cmath>
#include <cassert>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Enums/BlendModeEnum.h"
#include "Structs/ColorStruct.h"
#include "Math/Types/Vector4.h"

/// <summary>
/// レンダーテクスチャの基底クラス
/// </summary>
class BaseRenderTexture {
public:
	BaseRenderTexture();
	virtual ~BaseRenderTexture();

	// 初期化
	virtual void Initialize() = 0;

	// 描画
	void Draw();

	// クリアカラーを取得
	Vector4 GetClearColor();
	// RTVCPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

	// SrvIndexを取得
	uint32_t GetSrvIndex()const;

	// リソースを取得
	ID3D12Resource* GetResource();

protected:
	void Create(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4 clearColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

private:
	// レンダーテクスチャのリソースを作成
	void CreateResource();
	// RTVを作成
	void CreateRTV();
	// SRVを作成
	void CreateSRV();
protected:
	// レンダーテクスチャ用のリソース
	ComPtr<ID3D12Resource> resource_ = nullptr;

	DXGI_FORMAT format_;
	D3D12_RESOURCE_FLAGS resourceFlags_;
	Vector4 clearColor_;

	// RTVリソースのインデックス
	uint32_t rtvIndex_ = 0;
	// SRVリソースのインデックス
	uint32_t srvIndex_ = 0;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::None;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
};