#include "ShadowDepthTexture.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"


using namespace MAGIUtility;

ShadowDepthTexture::ShadowDepthTexture() {}

ShadowDepthTexture::~ShadowDepthTexture() {}

void ShadowDepthTexture::CreateResource() {
    constexpr uint32_t kShadowMapWidth = 2048;
    constexpr uint32_t kShadowMapHeight = 2048;

    // リソース作成
    resource_ = dxgi_->CreateDepthStencilTextureResource(
        kShadowMapWidth,
        kShadowMapHeight,
        DXGI_FORMAT_D32_FLOAT // 深度専用
    );

    // ビュー作成
    dsvIndex_ = srvuavManager_->Allocate();
    srvIndex_ = srvuavManager_->Allocate();

    srvuavManager_->CreateDSVTexture2d(dsvIndex_, resource_.Get(), DXGI_FORMAT_D32_FLOAT);
    srvuavManager_->CreateSrvDepthTexture2d(srvIndex_, resource_.Get(), DXGI_FORMAT_R32_FLOAT);

    currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
}

void ShadowDepthTexture::Clear() {
   
}

void ShadowDepthTexture::ClearRenderTarget() {
     MAGISYSTEM::GetDirectXCommandList()->ClearDepthStencilView(
        MAGISYSTEM::GetRTVDescriptorHandleCPU->GetDescriptorHandleCPU(dsvIndex_),
        D3D12_CLEAR_FLAG_DEPTH,
        1.0f,
        0,
        0,
        nullptr
    );
}

void ShadowDepthTexture::SetAsRenderTarget() {
    auto dsvHandle = srvuavManager_->GetDescriptorHandleCPU(dsvIndex_);
    MAGISYSTEM::GetDirectXCommandList()->OMSetRenderTargets(0, nullptr, FALSE, &dsvHandle);
}

void ShadowDepthTexture::TransitionToWrite() {
    if (currentResourceState_ != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = resource_.Get();
        barrier.Transition.StateBefore = currentResourceState_;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

        currentResourceState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    }
}

void ShadowDepthTexture::TransitionToRead() {
    if (currentResourceState_ != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) {
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = resource_.Get();
        barrier.Transition.StateBefore = currentResourceState_;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        MAGISYSTEM::GetDirectXCommandList()->ResourceBarrier(1, &barrier);

        currentResourceState_ = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE ShadowDepthTexture::GetDSVHandle() const {
    return MAGISYSTEM::->GetDescriptorHandleCPU(dsvIndex_);
}

D3D12_GPU_DESCRIPTOR_HANDLE ShadowDepthTexture::GetSRVHandle() const {
    return srvuavManager_->GetDescriptorHandleGPU(srvIndex_);
}
