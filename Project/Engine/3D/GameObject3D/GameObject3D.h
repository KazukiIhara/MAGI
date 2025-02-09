#pragma once

#include <memory>

#include "Structs/ObjectStruct.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"
#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/RenderObjects3D/PrimitiveRenderer3D/PrimitiveRenderer3D.h"
#include "3D/RenderObjects3D/StaticRenderer3D/StaticRenderer3D.h"
#include "3D/RenderObjects3D/SkinningRenderer3D/SkinningRenderer3D.h"

/// <summary>
/// 3Dゲームオブジェクトクラス
/// </summary>
class GameObject3D:public WorldEntity {
public:
	GameObject3D(const std::string& objectName, const EulerTransform3D& transform = EulerTransform3D{});
	~GameObject3D()override = default;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	// ワールドトランスフォームの取得
	WorldTransform* GetWorldTransform();

	// シンプル形状描画オブジェクト生成
	void CreatePrimitiveRenderer(const std::string& rendererName, const Primitive3DType& primitiveType, const std::string& textureName = "");
	// スキニングなしモデル描画オブジェクト生成
	void CreateStaticRenderer(const std::string& rendererName, const std::string& modelName);
	// スキニングありモデル描画オブジェクト生成
	void CreateSkinningRenderer(const std::string& rendererName, const std::string& modelName);
private:
	// ワールドトランスフォーム作成
	void CreateWorldTransform(const EulerTransform3D& transform);
private:
	// ワールド上の姿勢
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	// 描画用クラス
	std::unique_ptr<BaseRenderable3D> renderer3D_ = nullptr;
};