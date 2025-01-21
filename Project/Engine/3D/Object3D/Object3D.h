#pragma once

// C++
#include <memory>

#include "Structs/ObjectStruct.h"
#include "Enums/BlendModeEnum.h"

#include "3D/WorldTransform/WorldTransform.h"
#include "3D/Model/Model.h"

class Camera3D;

class Object3D {
public:
	Object3D(const std::string& modelName);
	~Object3D();
	void Initialize(const std::string& modelName);
	void Update();
	void Draw();

	void SetCamera(Camera3D* camera) {
		camera3d_ = camera;
	}

private:
	// WVP用のリソース作成
	void CreateWVPResource();
	// データを書き込む
	void MapWVPData();
	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMateiralData();

	// WVPデータの更新
	void UpdateWVPData();
	// マテリアルデータの更新
	void UpdateMaterialData();

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

private:
	// WVP用のリソース
	ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// シェーダーに送るWVPデータ
	TransformationMatrix* transformationData_ = nullptr;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material3D* materialData_ = nullptr;
	// マテリアル
	Material3D material_;

	// カメラを受け取る箱
	Camera3D* camera3d_ = nullptr;
};