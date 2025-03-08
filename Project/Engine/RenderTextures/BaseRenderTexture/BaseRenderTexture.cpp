#include "BaseRenderTexture.h"

#include "Framework/MAGI.h"


BaseRenderTexture::BaseRenderTexture() {

}

BaseRenderTexture::~BaseRenderTexture() {

}

void BaseRenderTexture::Initialize() {

}

void BaseRenderTexture::Draw() {

}

Vector4 BaseRenderTexture::GetClearColor() {
	return Vector4();
}

D3D12_CPU_DESCRIPTOR_HANDLE BaseRenderTexture::GetRTVHandle() {
	return D3D12_CPU_DESCRIPTOR_HANDLE();
}

ID3D12Resource* BaseRenderTexture::GetResource() {
	return nullptr;
}

void BaseRenderTexture::CreateResource() {
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(WindowApp::kClientWidth);				// Textureの幅
	resourceDesc.Height = UINT(WindowApp::kClientHeight);			// Textureの高さ
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;				// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;									// サンプリングカウント。1固定
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;		// renderTargetとして利用可能にする
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// クリアカラーの設定
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = kClearColor_.r;
	clearValue.Color[1] = kClearColor_.g;
	clearValue.Color[2] = kClearColor_.b;
	clearValue.Color[3] = kClearColor_.a;

	// リソースの作成
	resource_ = nullptr;
	hr_ = MAGISYSTEM::GetDirectXDevice()->CreateCommittedResource(
		&heapProperties,								// Heapの設定
		D3D12_HEAP_FLAG_NONE,							// Heapの特殊な設定。特になし。
		&resourceDesc,									// リソースの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,				// これから描画することを前提としたTextureなので、RenderTargetとして使うことから始める
		&clearValue,									// Clear最適値、ClearRenderをこの色でクリアするようにする、最適化されているので高速である。
		IID_PPV_ARGS(&resource_)
	);
	assert(SUCCEEDED(hr_));
}

void BaseRenderTexture::CreateRTV() {
	
}

void BaseRenderTexture::CreateSRV() {

}
