#pragma once

#include "Framework/MAGI.h"

// 実行するゲームクラス
class Game :public MAGISYSTEM {
public:
	~Game();
	// 更新
	void Update()override;
	// 描画
	void Draw()override;

};