#include "GameObject3DManager.h"

#include "Logger/Logger.h"

GameObject3DManager::GameObject3DManager() {
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObject3DManager::~GameObject3DManager() {
	Logger::Log("GameObject3DManager Finalize\n");
}

void GameObject3DManager::Update() {
	for (auto& gameObject : gameObjects_) {
		if (gameObject.second) {
			// オブジェクトごとの更新処理
			gameObject.second->Update();
			// ワールド行列の更新書影
			gameObject.second->UpdateWorldTransform();
		}
	}
}

void GameObject3DManager::Draw() {
	for (auto& gameObject : gameObjects_) {
		if (gameObject.second) {
			gameObject.second->Draw();
		}
	}
}

std::string GameObject3DManager::Create(std::unique_ptr<GameObject3D> newGameObject3D) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = newGameObject3D->name_;
	int suffix = 1;

	// 同じ名前が既に存在する場合、ユニークな名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(gameObjects_.begin(), gameObjects_.end(), [&](const auto& gameObject) {
			return gameObject.second->name_ == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = newGameObject3D->name_ + "_" + std::to_string(suffix);
		suffix++;
	}

	// ユニークな名前に改名
	newGameObject3D->name_ = uniqueName;

	// コンテナに登録
	gameObjects_.insert(std::pair(uniqueName, std::move(newGameObject3D)));

	// ユニークな名前を返す
	return uniqueName;
}

void GameObject3DManager::Remove(const std::string& name) {
	name;
}
