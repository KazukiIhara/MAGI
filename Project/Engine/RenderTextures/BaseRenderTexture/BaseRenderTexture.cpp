#include "BaseRenderTexture.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

BaseRenderTexture::BaseRenderTexture() {

}

BaseRenderTexture::~BaseRenderTexture() {

}

void BaseRenderTexture::Initialize(DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags, Vector4 clearColor) {
	format_ = format;
	resourceFlags_ = resourceFlags;
	clearColor_ = clearColor;
	// リソースを作成
	CreateResource();
	// RTVを作成
	CreateRTV();
	// SRVを作成
	CreateSRV();
}

void BaseRenderTexture::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// ディスクリプタテーブルを設定
	commandList->SetGraphicsRootDescriptorTable(0, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(srvIndex_));
	// ドローコール
	commandList->DrawInstanced(3, 1, 0, 0);
}

Vector4 BaseRenderTexture::GetClearColor() {
	return clearColor_;
}

D3D12_CPU_DESCRIPTOR_HANDLE BaseRenderTexture::GetRTVHandle() {
	return MAGISYSTEM::GetRTVDescriptorHandleCPU(rtvIndex_);
}

ID3D12Resource* BaseRenderTexture::GetResource() {
	return resource_.Get();
}

uint32_t BaseRenderTexture::GetSrvIndex() const {
	return srvIndex_;
}

void BaseRenderTexture::CreateResource() {
	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(WindowApp::kClientWidth);					// Textureの幅
	resourceDesc.Height = UINT(WindowApp::kClientHeight);				// Textureの高さ
	resourceDesc.Format = format_;										// TextureのFormat
	resourceDesc.SampleDesc.Count = 1;									// サンプリングカウント。1固定
	resourceDesc.Flags = resourceFlags_;		// renderTargetとして利用可能にする
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// クリアカラーの設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = clearColor_.x;
	clearValue.Color[1] = clearColor_.y;
	clearValue.Color[2] = clearColor_.z;
	clearValue.Color[3] = clearColor_.w;

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
	// インデックスを割り当て
	rtvIndex_ = MAGISYSTEM::RTVAllocate();
	// RTVを作成
	MAGISYSTEM::CreateRTVTexture2d(rtvIndex_, resource_.Get());
}

void BaseRenderTexture::CreateSRV() {
	// インデックス割り当て
	srvIndex_ = MAGISYSTEM::SrvUavAllocate();
	// SRVを作成
	MAGISYSTEM::CreateSrvTexture2D(srvIndex_, resource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);
}
