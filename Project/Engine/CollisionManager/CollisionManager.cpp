#include "CollisionManager.h"

#include <cassert>

#include "ColliderManager/ColliderManager.h"
#include "3D/GameObject3D/GameObject3D.h"

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
	// このフレームでチェックするアクティブなコライダーをリストに挿入

	// 全コライダー同士で衝突判定をとり、マップに挿入
	std::set<std::pair<uint32_t, uint32_t>> currentCollisions = CheckAllCollisions();

	// 衝突応答を呼ぶ
	ResolveCollisions(currentCollisions);

	// リスト内のコライダーを削除
	Clear();
}

void CollisionManager::Add(BaseCollider3D* baseCollider3D) {
	if (baseCollider3D) {
		colliders_.push_back(baseCollider3D);
	}
}

BaseCollider3D* CollisionManager::FindColliderByID(const std::vector<BaseCollider3D*>& colliders, uint32_t id) {
	for (auto* c : colliders) {
		if (c && c->GetID() == id) {
			return c;
		}
	}
	return nullptr;
}

bool CollisionManager::CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB) {
	// コライダーのタイプを取得
	Collider3DType typeA = colliderA->GetType();
	Collider3DType typeB = colliderB->GetType();

	// タイプの組み合わせを順序固定で取得
	auto key = MakeOrderedPair(typeA, typeB);

	// マップから衝突判定関数を探す
	auto it = collisionFuncMap_.find(key);
	if (it != collisionFuncMap_.end()) {
		// 見つかったら衝突判定関数を実行
		return it->second(colliderA, colliderB);
	} else {
		assert(false && "Warning: Not Found ColliderType");
		return false;
	}
}

std::set<std::pair<uint32_t, uint32_t>> CollisionManager::CheckAllCollisions() {
	std::set<std::pair<uint32_t, uint32_t>> collisions;

	// 総当たり or Broad-Phase で候補を抽出 etc.
	for (size_t i = 0; i < colliders_.size(); ++i) {
		BaseCollider3D* colliderA = colliders_[i];
		if (!colliderA || !colliderA->GetIsActive()) continue;

		for (size_t j = i + 1; j < colliders_.size(); ++j) {
			BaseCollider3D* colliderB = colliders_[j];
			if (!colliderB || !colliderB->GetIsActive()) continue;

			// 実際の衝突検出
			if (CheckCollisionPair(colliderA, colliderB)) {
				// 衝突していたらソートしたIDペアをセットに格納
				auto pair = MakeOrderedPair(colliderA->GetID(), colliderB->GetID());
				collisions.insert(pair);
			}
		}
	}

	return collisions;

}

void CollisionManager::ResolveCollisions(const std::set<std::pair<uint32_t, uint32_t>>& currentCollisions) {

	// 2-1. 新規衝突(OnCollisionEnter) or 継続衝突(OnCollisionStay)
	for (const auto& pair : currentCollisions) {
		// このペアが前フレームにも存在していたらStay, なければEnter
		bool isNew = (previousCollisions_.find(pair) == previousCollisions_.end());

		auto [idA, idB] = pair;
		BaseCollider3D* colliderA = FindColliderByID(colliders_, idA);
		BaseCollider3D* colliderB = FindColliderByID(colliders_, idB);
		if (!colliderA || !colliderB) {
			continue;
		}

		GameObject3D* ownerA = colliderA->GetOwner();
		GameObject3D* ownerB = colliderB->GetOwner();
		if (!ownerA || !ownerB) {
			continue;
		}

		if (isNew) {
			// 衝突開始
			ownerA->OnCollisionEnter(ownerB);
			ownerB->OnCollisionEnter(ownerA);
		} else {
			// 衝突継続
			ownerA->OnCollisionStay(ownerB);
			ownerB->OnCollisionStay(ownerA);
		}
	}

	// 2-2. 衝突離脱(OnCollisionExit)
	for (const auto& oldPair : previousCollisions_) {
		// 前フレームにあったペアが今フレームに無い場合は離脱
		if (currentCollisions.find(oldPair) == currentCollisions.end()) {
			auto [idA, idB] = oldPair;
			BaseCollider3D* colliderA = FindColliderByID(colliders_, idA);
			BaseCollider3D* colliderB = FindColliderByID(colliders_, idB);
			if (!colliderA || !colliderB) {
				continue;
			}

			GameObject3D* ownerA = colliderA->GetOwner();
			GameObject3D* ownerB = colliderB->GetOwner();
			if (!ownerA || !ownerB) {
				continue;
			}

			// 衝突終了
			ownerA->OnCollisionExit(ownerB);
			ownerB->OnCollisionExit(ownerA);
		}
	}
}

void CollisionManager::Clear() {
	colliders_.clear();
}

std::pair<uint32_t, uint32_t> CollisionManager::MakeOrderedPair(uint32_t idA, uint32_t idB) {
	if (idA < idB) {
		return { idA, idB };
	} else {
		return { idB, idA };
	}
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
