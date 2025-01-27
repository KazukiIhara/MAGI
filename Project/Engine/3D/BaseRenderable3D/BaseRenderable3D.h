#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

// MyHedder
#include "Structs/ObjectStruct.h"
#include "Enums/BlendModeEnum.h"

#include "DirectX/ComPtr/ComPtr.h"
#include "3D/WorldEntity/WorldEntity.h"
#include "3D/WorldTransform/WorldTransform.h"

class BaseRenderable3D :public WorldEntity {
public:
	BaseRenderable3D(const std::string& objectName);
	virtual ~BaseRenderable3D() = default;
	virtual void Update();
	virtual void Draw() = 0;

	Vector3& GetScale();
	Vector3& GetRotate();
	Vector3& GetTranslate();

	UVTransform& GetUvTransform();

	Material3DForGPU& GetMaterial();
protected:
	void Initialize(const std::string& objectName);
	// 描画前の処理
	void PrepareForRendering(bool isNormalMap = false);
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
	// UVトランスフォーム
	UVTransform uvTransform_{};
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
	Material3DForGPU* materialData_ = nullptr;
	// マテリアル
	Material3DForGPU material_{};
};