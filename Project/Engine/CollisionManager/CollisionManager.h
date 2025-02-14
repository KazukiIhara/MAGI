#pragma once

// C++
#include <map>
#include <functional>

// MyHedder
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
	void Add(BaseCollider3D* baseCollider3D);
	// ペア同士の当たり判定をチェック
	void CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB);
	// すべてのコライダー同士の当たり判定チェック
	void CheckAllCollisions();
	// コライダーリストをクリア
	void Clear();

private:
	// ペアの順序を固定する関数
	std::pair<Collider3DType, Collider3DType> MakeOrderedPair(Collider3DType typeA, Collider3DType typeB);

	// 球体同士の当たり判定
	bool CheckSphereToSphereCollision(BaseCollider3D* colliderA, BaseCollider3D* colliderB);

private:
	// 衝突判定関数の型の定義
	using CollisionFunc = std::function<bool(BaseCollider3D*, BaseCollider3D*)>;
	// 衝突判定関数を管理するマップ
	std::map<std::pair<Collider3DType, Collider3DType>, CollisionFunc> collisionFuncMap_;

};