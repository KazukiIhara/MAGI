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
#include "Enums/PostEffectPipelineEnum.h"
#include "Structs/ColorStruct.h"
#include "Math/Types/Vector4.h"

class BaseRenderTexture {
public:
	BaseRenderTexture();
	~BaseRenderTexture();

	// 初期化
	void Initialize();

	// 描画
	void Draw();

	// クリアカラーを取得
	Vector4 GetClearColor();

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
	// RTVリソースのインデックス
	uint32_t rtvIndex_ = 0;
	// SRVリソースのインデックス
	uint32_t srvIndex_ = 0;
	// クリアカラー
	RGBA kClearColor_ = Color::White;
	// ポストエフェクトのタイプ
	PostEffectPipelineStateType postEffectType_ = PostEffectPipelineStateType::None;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::None;
	// エラー判別君
	HRESULT hr_ = S_FALSE;
};