#pragma once

#include <vector>
#include <string>
#include <memory>

#include "3D/GameObject3D/GameObject3D.h"

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

	void Update();
	void Draw();

	std::string Create(const std::string& name);
	void Remove(const std::string& name);

private:
	// ゲームオブジェクトコンテナ
	std::vector<std::unique_ptr<GameObject3D>> gameObjects_;
};