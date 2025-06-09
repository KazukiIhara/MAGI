#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "GameObject3D/GameObject3D.h"

/// <summary>
/// 3Dゲームオブジェクトマネージャー
/// </summary>
class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	GameObject3D* Add(std::unique_ptr<GameObject3D> gameObject3D);
	void Update();
	void DeleteGarbage();
	void Clear();

private:
	std::vector<std::unique_ptr<GameObject3D>> gameObjects_;
};