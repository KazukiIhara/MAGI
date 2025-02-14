#pragma once

#include <memory>
#include <optional>

#include "Includes/Enums/Collider3DType.h"
#include "GameData/CollisionCategory.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

class GameObject3D;

class BaseCollider3D:public WorldEntity {
public:
	BaseCollider3D(GameObject3D* owner, Collider3DType type);
	virtual ~BaseCollider3D() = default;

	void Update();
	virtual void Draw() = 0;

	Collider3DType GetType()const;

protected:
	// 所属するゲームオブジェクトのポインタ
	GameObject3D* owner_ = nullptr;
	// コライダーのタイプ
	std::optional<Collider3DType> type_ = std::nullopt;
	// コライダーのカテゴリ
	CollisionCategory collisionCategory_ = CollisionCategory::None;
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
};