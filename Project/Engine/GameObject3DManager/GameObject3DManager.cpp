#include "GameObject3DManager.h"

#include "Logger/Logger.h"

GameObjectManager::GameObjectManager() {
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObjectManager::~GameObjectManager() {
	Logger::Log("GameObject3DManager Finalize\n");
}

void GameObjectManager::Update() {
	
}

void GameObjectManager::Draw() {
	
}

std::string GameObjectManager::Create(const std::string& name) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = name;
	int suffix = 1;

	// 同じ名前が既に存在する場合、一意な名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(gameObjects_.begin(), gameObjects_.end(), [&](const auto& gameObject) {
			return gameObject->name_ == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = name + "_" + std::to_string(suffix);
		suffix++;
	}

	// 追加するゲームオブジェクト
	std::unique_ptr<GameObject3D> newGameObject3D = std::make_unique<GameObject3D>(uniqueName);

	// コンテナに登録
	gameObjects_.push_back(std::move(newGameObject3D));

	return uniqueName;
}

void GameObjectManager::Remove(const std::string& name) {

}
