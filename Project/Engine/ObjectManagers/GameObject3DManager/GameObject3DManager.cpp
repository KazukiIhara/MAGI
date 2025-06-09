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

GameObject3D* GameObject3DManager::Add(std::unique_ptr<GameObject3D> gameObject3D) {
	assert(gameObject3D && "GameObject3D must not be null");
	GameObject3D* ptr = gameObject3D.get();
	gameObjects_.push_back(std::move(gameObject3D));
	return ptr;
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
	std::erase_if(gameObjects_, [](const std::unique_ptr<GameObject3D>& gameObject) {
		return gameObject && !gameObject->GetIsAlive();
		});

}

void GameObject3DManager::Clear() {
	gameObjects_.clear();
}