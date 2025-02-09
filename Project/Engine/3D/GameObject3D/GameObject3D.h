#pragma once

#include <memory>

#include "Interfaces/IRenderObject3D.h"

/// <summary>
/// 3Dゲームオブジェクトクラス
/// </summary>
class GameObject3D {
public:


private:
	// 3Dレンダラーのポインタ
	std::unique_ptr<IRenderObject3D> renderer3D_ = nullptr;

};