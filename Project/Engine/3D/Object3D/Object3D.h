#pragma once

// C++
#include <memory>

#include "Structs/ObjectStruct.h"
#include "Enums/BlendModeEnum.h"

#include "3D/WorldTransform/WorldTransform.h"
#include "3D/Model/Model.h"

class Object3D {
public:
	Object3D(const std::string& modelName);
	~Object3D();
	void Initialize();
	void Update();
	void Draw();

	ModelData GetModelData(const std::string& filePath);

private:
	// WVP用のリソース作成
	void CreateWVPResource();
	// データを書き込む
	void MapWVPData();
	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMateiralData();
private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;
};