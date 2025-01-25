#pragma once

// C++
#include <unordered_map>
#include <string>

// Assimp
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

// MyHedder
#include "Structs/SkeletonStruct.h"

/// <summary>
/// スケルトンデータコンテナ
/// </summary>
class SkeletonDataContainer {
public:
	SkeletonDataContainer();
	~SkeletonDataContainer();

	void Initialize();
	void Load(const std::string& skeletonName);

private:
	SkeletonData LoadSkeleton(const std::string& skeletonName);

private:
	// スケルトンデータ
	std::unordered_map<std::string, SkeletonData> skeletonDatas_;
};
