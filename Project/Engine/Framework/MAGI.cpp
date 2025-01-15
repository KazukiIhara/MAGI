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

std::unique_ptr<RTVManager> MAGISYSTEM::rtvManager_ = nullptr;
std::unique_ptr<DSVManager> MAGISYSTEM::dsvManager_ = nullptr;
std::unique_ptr<SRVUAVManager> MAGISYSTEM::srvuavManager_ = nullptr;

std::unique_ptr<SwapChain> MAGISYSTEM::swapChain_ = nullptr;
std::unique_ptr<DepthStencil> MAGISYSTEM::depthStencil_ = nullptr;
std::unique_ptr<ResourceBarrier> MAGISYSTEM::resourceBarrier_ = nullptr;

void MAGISYSTEM::Initialize() {
#ifdef _DEBUG
	// リークチェッカ
	leakCheck_ = std::make_unique<D3DResourceLeakChecker>();
#endif // _DEBUG

	// 開始ログ
	Logger::Log("MAGISYSTEM Initialize\n");

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

}

void MAGISYSTEM::Finalize() {

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
		windowApp_->Finalize();
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

}

void MAGISYSTEM::Draw() {

}

void MAGISYSTEM::PreDraw() {

}

void MAGISYSTEM::PostDraw() {

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

