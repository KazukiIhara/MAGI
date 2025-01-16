#include "Game.h"

Game::~Game() {}

void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();


}

void Game::Update() {
	// 基底システムの更新
	MAGISYSTEM::Update();	


}

void Game::Draw() {
	// 規定システムの描画
	MAGISYSTEM::Draw();


}
