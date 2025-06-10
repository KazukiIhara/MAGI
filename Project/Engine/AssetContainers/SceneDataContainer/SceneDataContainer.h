#pragma once

// C++
#include <string>
#include <vector>
#include <unordered_map>

// MyHedder
#include "Math/Types/Vector3.h"

struct SceneObjectData {
	std::string objectName;
	std::string modelName;
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct SceneData {
	std::vector<SceneObjectData> objects;
};

/// <summary>
/// シーンデータコンテナ
/// </summary>
class SceneDataContainer {
public:
	SceneDataContainer();
	~SceneDataContainer();

	// シーンデータをロード
	void Load(const std::string& fileName);

private:
	// シーンデータコンテナ
	std::unordered_map<std::string, SceneData> sceneDatas_;
};