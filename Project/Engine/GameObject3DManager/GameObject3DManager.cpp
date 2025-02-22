#include "GameObject3DManager.h"

#include "Logger/Logger.h"

GameObjectManager::GameObjectManager() {
	Logger::Log("GameObject3DManager Initialize\n");
}

GameObjectManager::~GameObjectManager() {
	Logger::Log("GameObject3DManager Finalize\n");
}
