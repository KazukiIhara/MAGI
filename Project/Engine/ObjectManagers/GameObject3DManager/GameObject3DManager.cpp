#include "GameObject3DManager.h"

#include <cassert>

// MyHedder
#include "Logger/Logger.h" 

GameObject3DManager::GameObject3DManager() {
	Clear();
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObject3DManager::~GameObject3DManager() {
	Clear();
	Logger::Log("GameObject3DManager Finalize\n");
}

std::weak_ptr<GameObject3D> GameObject3DManager::Add(std::shared_ptr<GameObject3D> gameObject3D) {
	assert(gameObject3D && "GameObject3D must not be null");
	gameObjects_.push_back(std::move(gameObject3D));
	return gameObjects_.back();
}

void GameObject3DManager::Update() {
	for (auto& gameobject : gameObjects_) {
		gameobject->Update();
	}
}

void GameObject3DManager::DeleteGarbage() {
	for (auto& gameobject : gameObjects_) {
		if (gameobject && !gameobject->GetIsAlive()) {
			gameobject->Finalize();
		}
	}

	std::erase_if(gameObjects_, [](const std::shared_ptr<GameObject3D>& gameObject) {
		return gameObject && !gameObject->GetIsAlive();
		});

}

void GameObject3DManager::Clear() {
	for (auto& gameobject : gameObjects_) {
		if (gameobject) {
			gameobject->Finalize();
		}
	}
	gameObjects_.clear();
}