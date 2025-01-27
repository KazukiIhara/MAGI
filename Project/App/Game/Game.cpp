#include "Game.h"

#include "Scenes/Sample/SampleScene.h"

Game::~Game() {}

void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();

	// シーンの追加
	sceneManager_->AddScene<SampleScene>("Sample");

	// 最初のシーンを設定
	sceneManager_->ChangeScene("Sample");
}
