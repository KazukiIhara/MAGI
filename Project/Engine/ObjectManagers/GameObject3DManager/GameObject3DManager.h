#pragma once

#include <map>
#include <string>
#include <memory>

#include "3D/GameObject3D/GameObject3D.h"

/// <summary>
/// 3Dゲームオブジェクトのマネージャー
/// </summary>
class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	void Update();
	void Draw();

	std::string Add(std::unique_ptr<GameObject3D> newGameObject);
	void Remove(const std::string& objectName);
	GameObject3D* Find(const std::string& objectName);

private:
	// ゲームオブジェクトコンテナ
	std::map<std::string, std::unique_ptr<GameObject3D>> gameObjects3D_;
};