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
	BaseCollider3D(Collider3DType type);
	virtual ~BaseCollider3D() = default;

	void Update();
	virtual void Draw() = 0;

	// コライダータイプの取得
	Collider3DType GetType()const;
	// オフセットの取得
	Vector3& GetOffset();

	// 所属するゲームオブジェクトのセット
	void SetOwner(GameObject3D* owner);

protected:
	// 所属するゲームオブジェクトのポインタ
	GameObject3D* owner_ = nullptr;
	// コライダーのタイプ
	std::optional<Collider3DType> type_ = std::nullopt;
	// コライダーのカテゴリ
	CollisionCategory collisionCategory_ = CollisionCategory::None;
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	// 親トランスフォームとのオフセット
	Vector3 offset_{};
};