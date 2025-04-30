#include "RenderPipelineController.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/PostEffectPipelineManager/PostEffectPipelineManager.h"


RenderController::RenderController(DXGI* dxgi, DirectXCommand* directXCommand, DepthStencil* depthStencil, Viewport* viewport, ScissorRect* scissorRect, SRVUAVManager* srvUavManager, PostEffectPipelineManager* postEffectPipelineManager) {
	// インスタンスを受け取る
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetDepthStencil(depthStencil);
	SetViewport(viewport);
	SetScissorRect(scissorRect);
	SetSrvUavManager(srvUavManager);
	SetPostEffectPipelineManager(postEffectPipelineManager);

	// パラメータ用のリソースを作成
	CreatePostEffectParamaterResource();

	// シーン描画用のレンダーテクスチャ
	sceneRenderTexture_ = std::make_unique<ColorRenderTexture>();
	sceneRenderTexture_->Initialize();

	// 最終描画用のレンダーテクスチャ
	finalRenderTexture_ = std::make_unique<ColorRenderTexture>();
	finalRenderTexture_->Initialize();

	// カラーポストエフェクト用のレンダーテクスチャ
	for (uint32_t i = 0; i < 2; i++) {
		colorPostEffectRenderTexture_[i] = std::make_unique<ColorRenderTexture>();
		colorPostEffectRenderTexture_[i]->Initialize();
	}

	// コマンドの最大数をあらかじめ決めておく
	postEffectCommand_.resize(kMaxPostEffectNum_);
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
	// 現在のテクスチャをシーン描画結果に
	currentRenderTexture_ = sceneRenderTexture_.get();
}

void RenderController::ApplyPostEffect() {
	// 今回積まれているポストエフェクトの数を取得
	const uint32_t currentFramePostEffectNum = currentCommandIndex_;
	if (currentFramePostEffectNum == 0) {
		return; // 何もなければリターン
	}

	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	for (uint32_t i = 0; i < currentFramePostEffectNum; i++) {
		const auto& command = postEffectCommand_[i];

		switch (command.postEffectType) {
		case PostEffectType::Copy:
		case PostEffectType::Grayscale:
		case PostEffectType::Vignette:
			DrawRenderTexture(commandList, command);
			break;
		}

		// 出力結果を次の入力にする
		currentRenderTexture_ = currentRenderTarget_;

		// 描画した対象を読み取り可能状態にする
		currentRenderTexture_->TransitionToRead();

	}

	// 最後の結果がcurrentRenderTexture_に入っている
	currentRenderTarget_ = nullptr; // 使い終わったのでnullに
	currentCommandIndex_ = 0; // コマンドバッファをリセット
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
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);

	// 最終レンダーテクスチャを読み取り可能状態にする
	finalRenderTexture_->TransitionToRead();
}

void RenderController::RenderToSwapChain() {
	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(PostEffectType::Copy));
	// PSOを設定
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(PostEffectType::Copy, BlendMode::None));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ディスクリプタハンドルを指定
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(finalRenderTexture_->GetSrvIndex()));

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderController::EndFrame() {
	// レンダーテクスチャをnullptrに
	currentRenderTarget_ = nullptr;
	currentRenderTexture_ = nullptr;

	// 次のフレーム用に書き込み可能状態にする
	sceneRenderTexture_->TransitionToWrite();
	finalRenderTexture_->TransitionToWrite();

	for (uint32_t i = 0; i < 2; i++) {
		colorPostEffectRenderTexture_[i]->TransitionToWrite();
	}

}

void RenderController::AddPostEffect(const PostEffectCommand& command) {
	// コマンドを追加
	postEffectCommand_[currentCommandIndex_] = command;
	// コマンドインデックスをインクリメント
	currentCommandIndex_++;
}

void RenderController::SwitchColorRenderTextureIndex() {
	if (!currentColorPostEffectRenderTextureIndex_) {
		currentColorPostEffectRenderTextureIndex_ = 1;
	} else {
		currentColorPostEffectRenderTextureIndex_ = 0;
	}
}

void RenderController::DrawRenderTexture(ID3D12GraphicsCommandList* commandList, PostEffectCommand command) {
	// 現在の書き込み先のレンダーテクスチャを切り替え
	currentRenderTarget_ = colorPostEffectRenderTexture_[currentColorPostEffectRenderTextureIndex_].get();
	// 次のポストエフェクト用にレンダーテクスチャを切り替え
	SwitchColorRenderTextureIndex();

	// レンダーターゲットを設定
	currentRenderTarget_->SetAsRenderTarget();
	currentRenderTarget_->ClearRenderTarget();

	// ビューポート、シザー設定
	viewport_->SettingViewport();
	scissorRect_->SettingScissorRect();

	// パラメータを更新
	postEffectParamData_ = &command.param;

	// ポストエフェクトに対応するパイプラインを設定
	commandList->SetGraphicsRootSignature(postEffectPipelineManager_->GetRootSignature(command.postEffectType));
	commandList->SetPipelineState(postEffectPipelineManager_->GetPipelineState(command.postEffectType, BlendMode::None));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 入力するテクスチャはひとつ前に描画したレンダーテクスチャ
	commandList->SetGraphicsRootDescriptorTable(0, srvUavManager_->GetDescriptorHandleGPU(currentRenderTexture_->GetSrvIndex()));

	// パラメータを送信
	commandList->SetGraphicsRootConstantBufferView(1, postEffectParamResource_->GetGPUVirtualAddress());

	// 描画
	commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderController::CreatePostEffectParamaterResource() {

}

void RenderController::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
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
