#pragma once

// C++
#include <map>
#include <string>
#include <unordered_map>

// DirectX
#include <d3d12.h>

// Assimp
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

// MyHedder
#include "Structs/ModelStruct.h"

// 前方宣言
class TextureDataContainer;

/// <summary>
/// モデルデータコンテナ
/// </summary>
class ModelDataContainer {
public:
	ModelDataContainer(TextureDataContainer* textureDataContainer);
	~ModelDataContainer();

	void Initialize(TextureDataContainer* textureDataContainer);
	void Load(const std::string& modelName, bool isNormalMap);

	ModelData FindModelData(const std::string& modelName)const;
private:
	ModelData LoadModel(const std::string& modelName, bool isNormalMap);
private:
	void SetTextureDataContainer(TextureDataContainer* textureDataContainer);
private:
	// モデルデータコンテナ
	std::unordered_map<std::string, ModelData> modelDatas_;
private:
	TextureDataContainer* textureDataContainer_ = nullptr;
};