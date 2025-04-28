#include "RenderPipelineController.h"

#include <cassert>

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"


RenderController::RenderController(DirectXCommand* directXCommand, DepthStencil* depthStencil, Viewport* viewport, ScissorRect* scissorRect, SRVUAVManager* srvUavManager, PostEffectPipelineManager* postEffectPipelineManager) {
	// インスタンスを受け取る
	SetDirectXCommand(directXCommand);
	SetDepthStencil(depthStencil);
	SetViewport(viewport);
	SetScissorRect(scissorRect);
	SetSrvUavManager(srvUavManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);

	// シーン描画用のレンダーテクスチャ
	sceneRenderTexture_ = std::make_unique<ColorRenderTexture>();
	sceneRenderTexture_->Initialize();

	// 最終描画用のレンダーテクスチャ
	finalRenderTexture_ = std::make_unique<ColorRenderTexture>();
	finalRenderTexture_->Initialize();
}

RenderController::~RenderController() {

}

void RenderController::PreSceneRender() {
	// レンダーターゲットをシーン描画用のレンダーテクスチャに指定
	sceneRenderTexture_->SetAsRenderTarget(depthStencil_->GetDepthStencilResorceCPUHandle());
	sceneRenderTexture_->ClearRenderTarget();
	// 深度をクリア
	depthStencil_->ClearDepthView();
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();
}

void RenderController::PostSceneRender() {
	// シーン描画用のレンダーターゲットを読み取り状態に
	sceneRenderTexture_->TransitionToRead();
}

void RenderController::ApplyPostEffect() {

}

void RenderController::RenderToFinalRenderTexture() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// レンダーターゲットを最終描画用のレンダーテクスチャに指定
	finalRenderTexture_->SetAsRenderTarget();
	finalRenderTexture_->ClearRenderTarget();
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();

	// 最終レンダーテクスチャに描画
	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectPipelineStateType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectPipelineStateType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(sceneRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 最終レンダーテクスチャを読み取り可能状態にする
	finalRenderTexture_->TransitionToRead();

}

void RenderController::RenderToSwapChain() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectPipelineStateType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectPipelineStateType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(finalRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderController::EndFrame() {
	// 次のフレーム用に書き込み可能状態にする
	sceneRenderTexture_->TransitionToWrite();
	finalRenderTexture_->TransitionToWrite();
}

void RenderController::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RenderController::SetDepthStencil(DepthStencil* depthStencil) {
	assert(depthStencil);
	depthStencil_ = depthStencil;
}

void RenderController::SetViewport(Viewport* viewport) {
	assert(viewport);
	viewport_ = viewport;
}

void RenderController::SetScissorRect(ScissorRect* scissorRect) {
	assert(scissorRect);
	scissorRect_ = scissorRect;
}

void RenderController::SetSrvUavManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void RenderController::SetPostEffectPipelineManager(PostEffectPipelineManager* postEffectPipelineManager) {
	assert(postEffectPipelineManager);
	postEffectPipelineManager_ = postEffectPipelineManager;
}
