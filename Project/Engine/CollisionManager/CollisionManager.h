#pragma once

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"

/// <summary>
/// コリジョンマネージャ
/// </summary>
class CollisionManager {
public:
	CollisionManager();
	~CollisionManager() = default;

	// 更新
	void Update();
	// チェックするコライダーを追加
	void AddCollider(BaseCollider3D* baseCollider3D);
	// ペア同士の当たり判定をチェック
	void CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB);
	// すべてのコライダー同士の当たり判定チェック
	void CheckAllCollisions();

private:
};