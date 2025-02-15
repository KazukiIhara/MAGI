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
	BaseCollider3D(uint32_t id, Collider3DType type);
	virtual ~BaseCollider3D() = default;

	void Update();
	virtual void Draw() = 0;

	// コライダータイプの取得
	Collider3DType GetType()const;
	// 識別IDの取得
	uint32_t GetID()const;

	// オフセットの取得
	Vector3& GetOffset();
	// アクティブフラグの取得
	bool& GetIsActive();

	// 所属するゲームオブジェクトのセット
	void SetOwner(GameObject3D* owner);

protected:
	// 所属するゲームオブジェクトのポインタ
	GameObject3D* owner_ = nullptr;
	// 識別ID
	uint32_t id_ = 0;
	// コライダーのタイプ
	std::optional<Collider3DType> type_ = std::nullopt;
	// コライダーのカテゴリ
	CollisionCategory collisionCategory_ = CollisionCategory::None;
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	// 親トランスフォームとのオフセット
	Vector3 offset_{};
	// アクティブフラグ
	bool isActive_ = false;
};