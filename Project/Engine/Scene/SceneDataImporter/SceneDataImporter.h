#pragma once

// C++
#include <string>

// 前方宣言
class SceneDataContainer;
class GameObject3DManager;

/// <summary>
/// シーンデータをインポートする
/// </summary>
class SceneDataImporter {
public:
	SceneDataImporter(SceneDataContainer* sceneDataContainer, GameObject3DManager* gameObject3DManager);
	~SceneDataImporter();

	// シーンデータインポート
	void Import(const std::string& scaneDataName, bool isSceneClear);

private:
	SceneDataContainer* sceneDataContainer_ = nullptr;
	GameObject3DManager* gameObject3DManager_ = nullptr;
};