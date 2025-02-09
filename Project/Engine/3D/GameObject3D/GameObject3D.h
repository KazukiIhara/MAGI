#pragma once

#include <memory>

#include "Enums/Renderer3DEnum.h"
#include "Structs/ObjectStruct.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"
#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"

/// <summary>
/// 3Dゲームオブジェクトクラス
/// </summary>
class GameObject3D:public WorldEntity {
public:
	GameObject3D(const std::string& objectName);
	~GameObject3D()override = default;

	virtual void Initialize(const EulerTransform3D& transform = EulerTransform3D{});
	virtual void Update();
	virtual void Draw();

	WorldTransform* GetWorldTransform();

	// ワールドトランスフォーム作成
	void CreateWorldTransform(const EulerTransform3D& transform);
	
	void CreatePrimitiveRenderer();
	void CreateStaticRenderer();
	void CreateSkinningRenderer();

private:
	// ワールド上の姿勢
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	// 描画用クラス
	std::unique_ptr<BaseRenderable3D> renderer3D_ = nullptr;
};