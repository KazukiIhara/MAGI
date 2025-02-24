#pragma once

#include <vector>
#include <string>
#include <memory>

#include "3D/GameObject3D/GameObject3D.h"

class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	void Update();
	void Draw();

	std::string Create(const std::string& name);
	void Remove(const std::string& name);

private:
	// ゲームオブジェクトコンテナ
	std::vector<std::unique_ptr<GameObject3D>> gameObjects_;
};