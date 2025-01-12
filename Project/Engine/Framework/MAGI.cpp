#include "MAGI.h"

#include "Logger/Logger.h"

// Staticメンバ変数
#ifdef _DEBUG
std::unique_ptr<D3DResourceLeakChecker> MAGISYSTEM::leakCheck_ = nullptr;
#endif // _DEBUG

// Staticメンバ変数の初期化
std::unique_ptr<WindowApp> MAGISYSTEM::windowApp_ = nullptr;
std::unique_ptr<DirectInput> MAGISYSTEM::directInput_ = nullptr;

void MAGISYSTEM::Initialize() {
	// 開始ログ
	Logger::Log("MAGISYSTEM Initialize\n");

	// WindowApp
	windowApp_ = std::make_unique<WindowApp>();
	windowApp_->Initialize();
	// DirectInput
	directInput_ = std::make_unique<DirectInput>();
	directInput_->Initialize(windowApp_.get());

}

void MAGISYSTEM::Finalize() {



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

