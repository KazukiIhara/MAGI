#pragma once

#include <memory>
#include <optional>

#include "Includes/Enums/Collider3DType.h"
#include "GameData/CollisionCategory.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

class GameObject3D;

class Collider3D :public WorldEntity {
public:
	Collider3D(GameObject3D* owner, Collider3DType type);
	~Collider3D();

	void Update();
	void Draw();

private:

private:
	// 所属するゲームオブジェクトのポインタ
	GameObject3D* owner_ = nullptr;
	// コライダーのタイプ
	std::optional<Collider3DType> type_ = std::nullopt;
	// コライダーのカテゴリ
	CollisionCategory collisionCategory_ = CollisionCategory::None;
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
};