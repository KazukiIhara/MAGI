#include "SceneDataImporter.h"

#include <cassert>
#include "Logger/Logger.h"

#include "SceneDataContainer/SceneDataContainer.h"
#include "GameObject3DManager/GameObject3DManager.h"

SceneDataImporter::SceneDataImporter(SceneDataContainer* sceneDataContainer, GameObject3DManager* gameObject3DManager) {
	assert(gameObject3DManager);
	gameObject3DManager_ = gameObject3DManager;
	Logger::Log("SceneDataImporter Initialize\n");
}

SceneDataImporter::~SceneDataImporter() {
	Logger::Log("SceneDataImporter Finalize\n");
}

void SceneDataImporter::Import(const std::string& scaneDataName, bool isSceneClear) {
	Logger::Log("Begin Import SceneData :" + scaneDataName + "\n");

	if (isSceneClear) {
		gameObject3DManager_->Clear();
	}



}
