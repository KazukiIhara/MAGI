#include "MAGI.h"

#include "Logger/Logger.h"

// Staticメンバ変数
#ifdef _DEBUG
std::unique_ptr<D3DResourceLeakChecker> MAGISYSTEM::leakCheck_ = nullptr;
#endif // _DEBUG

// Staticメンバ変数の初期化
std::unique_ptr<WindowApp> MAGISYSTEM::windowApp_ = nullptr;


void MAGISYSTEM::Initialize() {
	// 開始ログ
	Logger::Log("MAGISYSTEM Initialize\n");

	// WindowApp
	windowApp_ = std::make_unique<WindowApp>();
	windowApp_->Initialize();

}

void MAGISYSTEM::Finalize() {



	// WindowApp
	if (windowApp_) {
		windowApp_->Finalize();
		windowApp_.reset();
	}

	// 終了ログ
	Logger::Log("MAGISYSTEM Finalize\n");
}

bool MAGISYSTEM::IsEndRequest() const {
	return endRequest_;
}

void MAGISYSTEM::Update() {
	// WindowAppの更新処理
	windowApp_->Update();

	// ウィンドウにメッセージが来ていたら最優先で処理
	if (windowApp_->ProcessMessage()) {
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

