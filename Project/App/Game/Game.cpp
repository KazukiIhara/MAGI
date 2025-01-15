#include "Game.h"

Game::~Game() {}

void Game::Update() {
	// 基底システムの更新
	MAGISYSTEM::Update();	
}

void Game::Draw() {
	// 描画
	MAGISYSTEM::Draw();
}
