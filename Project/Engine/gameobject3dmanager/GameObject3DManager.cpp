#include "GameObject3DManager.h"

#include "Logger/Logger.h"

#include <cassert>

GameObject3DManager::GameObject3DManager() {
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObject3DManager::~GameObject3DManager() {
	Logger::Log("GameObject3DManager Finalize\n");
}

void GameObject3DManager::Update() {
	for (auto& gameObject : gameObjects3D_) {
		if (gameObject.second) {
			// オブジェクトごとの更新処理
			gameObject.second->Update();
			// ワールド行列の更新書影
			gameObject.second->UpdateWorldTransform();
		}
	}
}

void GameObject3DManager::Draw() {
	for (auto& gameObject : gameObjects3D_) {
		if (gameObject.second) {
			// 描画処理
			gameObject.second->Draw();
		}
	}
}

std::string GameObject3DManager::Add(std::unique_ptr<GameObject3D> newGameObject3D) {
	// 新しいオブジェクト名を決定
	std::string uniqueName = newGameObject3D->name;
	int suffix = 1;

	// 同じ名前が既に存在する場合、ユニークな名前を生成
	auto isNameUsed = [&](const std::string& testName) {
		return std::any_of(gameObjects3D_.begin(), gameObjects3D_.end(), [&](const auto& gameObject) {
			return gameObject.second->name == testName;
			});
		};

	while (isNameUsed(uniqueName)) {
		uniqueName = newGameObject3D->name + "_" + std::to_string(suffix);
		suffix++;
	}

	// ユニークな名前に改名
	newGameObject3D->name = uniqueName;

	// コンテナに登録
	gameObjects3D_.insert(std::pair(uniqueName, std::move(newGameObject3D)));

	// ユニークな名前を返す
	return uniqueName;
}

void GameObject3DManager::Remove(const std::string& objectName) {
	// 指定した名前のオブジェクトを検索
	if (gameObjects3D_.contains(objectName)) {
		// 見つかったら消す
		gameObjects3D_.erase(objectName);
	} else {
		// 指定した名前のオブジェクトがない場合止める
		assert(false && "GameObject3D Not Found");
	}
}

GameObject3D* GameObject3DManager::Find(const std::string& objectName) {
	// 指定した名前のオブジェクトを検索
	auto it = gameObjects3D_.find(objectName);

	// 見つかった場合はポインタを返す
	if (it != gameObjects3D_.end()) {
		return it->second.get();
	}

	// 見つからなかった場合は nullptr を返す
	return nullptr;
}