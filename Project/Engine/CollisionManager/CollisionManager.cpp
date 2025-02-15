#include "CollisionManager.h"

#include <cassert>

#include "ColliderManager/ColliderManager.h"

CollisionManager::CollisionManager(ColliderManager* colliderManager) {
	// コライダーマネージャのインスタンスをセット
	assert(colliderManager);
	colliderManager_ = colliderManager;

	//
	// 各コライダーの組み合わせに対応する衝突判定関数を登録する
	//

	// 球体同士
	collisionFuncMap_[{Collider3DType::Sphere, Collider3DType::Sphere}] =
		[this](BaseCollider3D* collider1, BaseCollider3D* collider2) {
		return this->CheckSphereToSphereCollision(collider1, collider2);
		};

}

void CollisionManager::Update() {
	// このフレームでチェックするコライダーをリストに挿入


}

void CollisionManager::Add(BaseCollider3D* baseCollider3D) {
	baseCollider3D;
}

void CollisionManager::CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB) {
	// コライダーのタイプを取得
	Collider3DType typeA = colliderA->GetType();
	Collider3DType typeB = colliderB->GetType();

	// タイプの組み合わせを順序固定で取得
	auto key = MakeOrderedPair(typeA, typeB);

	// マップから衝突判定関数を探す
	auto it = collisionFuncMap_.find(key);
	if (it != collisionFuncMap_.end()) {
		// 見つかったら衝突判定関数を実行
		if (it->second(colliderA, colliderB)) {
			// 衝突処理

		}
	}
}

void CollisionManager::CheckAllCollisions() {

}

void CollisionManager::Clear() {

}

std::pair<Collider3DType, Collider3DType> CollisionManager::MakeOrderedPair(Collider3DType typeA, Collider3DType typeB) {
	if (typeA < typeB) {
		return { typeA, typeB };
	} else {
		return { typeB, typeA };
	}
}

bool CollisionManager::CheckSphereToSphereCollision(BaseCollider3D* colliderA, BaseCollider3D* colliderB) {
	colliderA;
	colliderB;
	return true;
}
