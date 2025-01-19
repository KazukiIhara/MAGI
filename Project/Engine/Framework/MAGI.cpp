#include "MAGI.h"

#include "Logger/Logger.h"

// Staticメンバ変数の初期化
#ifdef _DEBUG
std::unique_ptr<D3DResourceLeakChecker> MAGISYSTEM::leakCheck_ = nullptr;
#endif // _DEBUG

std::unique_ptr<WindowApp> MAGISYSTEM::windowApp_ = nullptr;
std::unique_ptr<DirectInput> MAGISYSTEM::directInput_ = nullptr;

std::unique_ptr<DXGI> MAGISYSTEM::dxgi_ = nullptr;
std::unique_ptr<DirectXCommand> MAGISYSTEM::directXCommand_ = nullptr;
std::unique_ptr<Fence> MAGISYSTEM::fence_ = nullptr;
std::unique_ptr<ShaderCompiler> MAGISYSTEM::shaderCompiler_ = nullptr;

std::unique_ptr<RTVManager> MAGISYSTEM::rtvManager_ = nullptr;
std::unique_ptr<DSVManager> MAGISYSTEM::dsvManager_ = nullptr;
std::unique_ptr<SRVUAVManager> MAGISYSTEM::srvuavManager_ = nullptr;

std::unique_ptr<SwapChain> MAGISYSTEM::swapChain_ = nullptr;
std::unique_ptr<DepthStencil> MAGISYSTEM::depthStencil_ = nullptr;
std::unique_ptr<ResourceBarrier> MAGISYSTEM::resourceBarrier_ = nullptr;
std::unique_ptr<RenderTarget> MAGISYSTEM::renderTarget_ = nullptr;
std::unique_ptr<Viewport> MAGISYSTEM::viewport_ = nullptr;
std::unique_ptr<ScissorRect> MAGISYSTEM::scissorRect_ = nullptr;

std::unique_ptr<GraphicsPipelineManager> MAGISYSTEM::graphicsPipelineManager_ = nullptr;

std::unique_ptr<SceneManager<GameData>> MAGISYSTEM::sceneManager_ = nullptr;


std::unique_ptr<ImGuiController> MAGISYSTEM::imguiController_ = nullptr;

void MAGISYSTEM::Initialize() {
	// 開始ログ
	Logger::Log("MAGISYSTEM Start\n");

#ifdef _DEBUG
	// リークチェッカ
	leakCheck_ = std::make_unique<D3DResourceLeakChecker>();
#endif // _DEBUG

	// WindowApp
	windowApp_ = std::make_unique<WindowApp>();
	// DirectInput
	directInput_ = std::make_unique<DirectInput>(windowApp_.get());

	// DXGI
	dxgi_ = std::make_unique<DXGI>();
	// DirectXCommand
	directXCommand_ = std::make_unique<DirectXCommand>(dxgi_.get());
	// Fence
	fence_ = std::make_unique<Fence>(dxgi_.get(), directXCommand_.get());
	// ShaderCompiler
	shaderCompiler_ = std::make_unique<ShaderCompiler>();

	// RTVManager
	rtvManager_ = std::make_unique<RTVManager>(dxgi_.get());
	// DSVManager
	dsvManager_ = std::make_unique<DSVManager>(dxgi_.get());
	// SRVUAVmanager
	srvuavManager_ = std::make_unique<SRVUAVManager>(dxgi_.get());

	// SwapChain
	swapChain_ = std::make_unique<SwapChain>(windowApp_.get(), dxgi_.get(), directXCommand_.get(), rtvManager_.get());
	// DepthStencil
	depthStencil_ = std::make_unique<DepthStencil>(dxgi_.get(), directXCommand_.get(), dsvManager_.get());
	// ResouceBarrier
	resourceBarrier_ = std::make_unique<ResourceBarrier>(directXCommand_.get(), swapChain_.get());
	// RenderTarget
	renderTarget_ = std::make_unique<RenderTarget>(directXCommand_.get(), swapChain_.get(), depthStencil_.get());
	// Viewport
	viewport_ = std::make_unique<Viewport>(directXCommand_.get());
	// Scissor
	scissorRect_ = std::make_unique<ScissorRect>(directXCommand_.get());

	// GraphicsPipelineManager
	graphicsPipelineManager_ = std::make_unique<GraphicsPipelineManager>(dxgi_.get(), shaderCompiler_.get());

	// SceneManager
	sceneManager_ = std::make_unique<SceneManager<GameData>>();



	// ImGuiController
	imguiController_ = std::make_unique<ImGuiController>(windowApp_.get(), dxgi_.get(), directXCommand_.get(), srvuavManager_.get());

	// 初期化完了ログ
	Logger::Log("MAGISYSTEM Initialize\n");
}

void MAGISYSTEM::Finalize() {

	// ImGuiController
	if (imguiController_) {
		imguiController_.reset();
	}


	// SceneManager
	if (sceneManager_) {
		sceneManager_.reset();
	}

	// GraphicsPipelineManager
	if (graphicsPipelineManager_) {
		graphicsPipelineManager_.reset();
	}

	// Scissor
	if (scissorRect_) {
		scissorRect_.reset();
	}

	// Viewport
	if (viewport_) {
		viewport_.reset();
	}

	// RenderTarget
	if (renderTarget_) {
		renderTarget_.reset();
	}

	// ResourceBarrier
	if (resourceBarrier_) {
		resourceBarrier_.reset();
	}

	// DepthStencil
	if (depthStencil_) {
		depthStencil_.reset();
	}

	// SwapChain
	if (swapChain_) {
		swapChain_.reset();
	}

	// SRVUAVManager
	if (srvuavManager_) {
		srvuavManager_.reset();
	}

	// DSVManager
	if (dsvManager_) {
		dsvManager_.reset();
	}

	// RTVManager
	if (rtvManager_) {
		rtvManager_.reset();
	}

	// ShaderCompiler
	if (shaderCompiler_) {
		shaderCompiler_.reset();
	}

	// Fence
	if (fence_) {
		fence_.reset();
	}

	// DirectXCommand
	if (directXCommand_) {
		directXCommand_.reset();
	}

	// DXGI
	if (dxgi_) {
		dxgi_.reset();
	}

	// DirectInput
	if (directInput_) {
		directInput_.reset();
	}

	// WindowApp
	if (windowApp_) {
		windowApp_.reset();
	}

	// 終了ログ
	Logger::Log("MAGISYSTEM Finalize\n");
}

void MAGISYSTEM::Update() {
	// ウィンドウにメッセージが来ていたら最優先で処理
	if (windowApp_->Update()) {
		endRequest_ = true;
	}

	// 入力の更新
	directInput_->Update();

	// F11キーでフルスクリーンの切り替え処理
	if (directInput_->TriggerKey(DIK_F11)) {
		windowApp_->ToggleFullScreen();
	}

	// escキーが入力されたらアプリケーションを終了
	if (directInput_->PushKey(DIK_ESCAPE)) {
		endRequest_ = true;
	}

	// ImGui開始処理
	imguiController_->BeginFrame();


	// シーンの更新処理
	sceneManager_->Update();

	// ImGui内部コマンド生成
	imguiController_->EndFrame();
}

void MAGISYSTEM::Draw() {

	// 
	// DirectX描画前処理
	// 

	// リソースバリアを設定
	resourceBarrier_->PreDrawSwapChainResourceBarrierTransition();
	// レンダーターゲットをスワップチェーンに設定
	renderTarget_->SetRenderTarget(RenderTargetType::SwapChain);
	// 深度をクリア
	depthStencil_->ClearDepthView();
	// レンダーターゲットをクリア
	renderTarget_->ClearRenderTarget(RenderTargetType::SwapChain);
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();

	// SRVUAVのディスクリプタヒープを設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvuavManager_->GetDescriptorHeap() };
	directXCommand_->GetList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// 
	// 描画処理
	// 

	//
	// Object3D描画前処理
	//


	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommand_->GetList()->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Object3D));
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	directXCommand_->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	// シーンの描画処理
	//
	sceneManager_->Draw();


	//
	// ImGui描画処理
	//

	imguiController_->Draw();


	// 
	// DirectX描画後処理
	// 

	// リソースバリアを描画後の状態にする
	resourceBarrier_->PostDrawSwapChainResourceBarrierTransition();
	// コマンドを閉じて実行
	directXCommand_->KickCommand();
	// GPUとOSに画面の交換を行うように通知
	swapChain_->Present();
	// Fenceによる待機
	fence_->WaitGPU();
	// 次のフレーム用にコマンドをリセット
	directXCommand_->ResetCommand();
}

void MAGISYSTEM::Run() {
	// 初期化
	Initialize();
	// メインループ
	while (true) {
		// 更新
		Update();

		// 終了リクエストがあったらループを抜ける;
		if (IsEndRequest()) {
			break;
		}

		// 描画
		Draw();
	}

	// 終了
	Finalize();
}

bool MAGISYSTEM::IsEndRequest() const {
	return endRequest_;
}

HWND MAGISYSTEM::GetWindowHandle() {
	return windowApp_->GetHwnd();
}

bool MAGISYSTEM::PushKey(BYTE keyNumber) {
	return directInput_->PushKey(keyNumber);
}

bool MAGISYSTEM::TriggerKey(BYTE keyNumber) {
	return directInput_->TriggerKey(keyNumber);
}

bool MAGISYSTEM::HoldKey(BYTE keyNumber) {
	return directInput_->HoldKey(keyNumber);
}

bool MAGISYSTEM::ReleaseKey(BYTE keyNumber) {
	return directInput_->ReleaseKey(keyNumber);
}

bool MAGISYSTEM::PushMouseButton(MouseButton mouseButton) {
	return directInput_->PushMouseButton(mouseButton);
}

bool MAGISYSTEM::TriggerMouseButton(MouseButton mouseButton) {
	return directInput_->TriggerMouseButton(mouseButton);
}

bool MAGISYSTEM::HoldMouseButton(MouseButton mouseButton) {
	return directInput_->HoldMouseButton(mouseButton);
}

bool MAGISYSTEM::ReleaseMouseButton(MouseButton mouseButton) {
	return directInput_->ReleaseMouseButton(mouseButton);
}

int64_t MAGISYSTEM::GetMouseWheelDelta() {
	return directInput_->GetMouseWheelDelta();
}

ID3D12GraphicsCommandList* MAGISYSTEM::GetDirectXCommandList() {
	return directXCommand_->GetList();
}

