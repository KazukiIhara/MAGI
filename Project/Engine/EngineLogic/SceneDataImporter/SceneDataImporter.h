#pragma once

// C++
#include <string>

// 前方宣言
class GameObject3DManager;

/// <summary>
/// シーンデータをインポートする
/// </summary>
class SceneDataImporter {
public:
	SceneDataImporter(GameObject3DManager* gameObject3DManager);
	~SceneDataImporter();

	// シーンデータインポート
	void Import(const std::string& scaneDataName, bool isSceneClear = true);

private:
	GameObject3DManager* gameObject3DManager_ = nullptr;
};