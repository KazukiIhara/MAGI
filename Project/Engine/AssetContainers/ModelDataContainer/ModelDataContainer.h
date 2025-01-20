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

/// <summary>
/// モデルデータコンテナ
/// </summary>
class ModelDataContainer {
public:
	ModelDataContainer();
	~ModelDataContainer();

	void Initialize();
	void Load(const std::string& fileName);

	ModelData FindModelData(const std::string& fileName)const;
private:
	ModelData LoadModel(const std::string& fileName);

private:
	// モデルデータコンテナ
	std::unordered_map<std::string, ModelData> modelDatas_;
};