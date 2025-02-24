#pragma once

#include <map>
#include <string>
#include <memory>

#include "3D/GameObject3D/GameObject3D.h"

class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	void Update();
	void Draw();

	std::string Create(std::unique_ptr<GameObject3D> newGameObject);
	void Remove(const std::string& name);

private:
	// ゲームオブジェクトコンテナ
	std::map<std::string, std::unique_ptr<GameObject3D>> gameObjects_;
};