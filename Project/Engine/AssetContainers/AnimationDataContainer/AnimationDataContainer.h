#pragma once

// C++
#include <string>
#include <unordered_map>

// Assimp
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

// MyHedder
#include "Structs/AnimationStruct.h"

/// <summary>
/// アニメーションのデータコンテナ
/// </summary>
class AnimationDataContainer {
public:
	AnimationDataContainer();
	~AnimationDataContainer();

	void Initialize();
	void Load(const std::string& animationFileName, bool isInSameDirectoryAsModel);

	AnimationData FindAnimationData(const std::string& animationName);
private:
	AnimationData LoadAnimation(const std::string& animationFileName, bool isInSameDirectoryAsModel);
private:
	// アニメーションデータコンテナ
	std::unordered_map<std::string, AnimationData> animationDatas_;
};