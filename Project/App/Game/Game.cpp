#include "Game.h"

Game::~Game() {}

void Game::Update() {
	// 基底システムの更新
	MAGISYSTEM::Update();
	
}

void Game::Draw() {
	// 描画前処理
	MAGISYSTEM::PreDraw();
	// 描画
	MAGISYSTEM::Draw();
	// 描画後処理
	MAGISYSTEM::PostDraw();
}
