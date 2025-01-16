#pragma once

#include "BaseScene/BaseScene.h"
#include <iostream>

// サンプルシーン
template <typename Data>
class TitleScene: public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};


