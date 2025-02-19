#include "CollisionManager.h"

#include <cassert>

#include "ColliderManager/ColliderManager.h"
#include "3D/GameObject3D/GameObject3D.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

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
	AddCheckColliders();
	// 全コライダー同士で衝突判定をとり、マップに挿入
	std::set<std::pair<uint32_t, uint32_t>> currentCollisions = CheckAllCollisions();

	// 衝突応答を呼ぶ
	ResolveCollisions(currentCollisions);

	// リスト内のコライダーを削除
	Clear();
}

void CollisionManager::Add(BaseCollider3D* baseCollider3D) {
	if (baseCollider3D) {
		activeColliders_.push_back(baseCollider3D);
	}
}

void CollisionManager::AddCheckColliders() {
	// 要素数を確保
	const auto& allColliders = colliderManager_->GetColliders();
	activeColliders_.reserve(allColliders.size());  // 大きさ分を確保

	for (auto& collider : allColliders) {
		if (collider && collider->GetIsActive()) {
			Add(collider.get());
		}
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

	// 総当たり
	for (size_t i = 0; i < activeColliders_.size(); ++i) {
		BaseCollider3D* colliderA = activeColliders_[i];
		if (!colliderA || !colliderA->GetIsActive()) continue;

		for (size_t j = i + 1; j < activeColliders_.size(); ++j) {
			BaseCollider3D* colliderB = activeColliders_[j];
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

	// 新規衝突(OnCollisionEnter) or 継続衝突(OnCollisionStay)
	for (const auto& pair : currentCollisions) {
		// このペアが前フレームにも存在していたらStay, なければEnter
		bool isNew = (previousCollisions_.find(pair) == previousCollisions_.end());

		auto [idA, idB] = pair;
		BaseCollider3D* colliderA = FindColliderByID(activeColliders_, idA);
		BaseCollider3D* colliderB = FindColliderByID(activeColliders_, idB);
		if (!colliderA || !colliderB) {
			continue;
		}

		if (isNew) {
			// 衝突開始
			colliderA->GetIsSellingOther() = true;
			colliderB->GetIsSellingOther() = true;
		}

		GameObject3D* ownerA = colliderA->GetOwner();
		GameObject3D* ownerB = colliderB->GetOwner();
		if (!ownerA || !ownerB) {
			continue;
		}

		if (isNew) {
			ownerA->OnCollisionEnter(ownerB);
			ownerB->OnCollisionEnter(ownerA);
		} else {
			// 衝突継続
			ownerA->OnCollisionStay(ownerB);
			ownerB->OnCollisionStay(ownerA);
		}
	}

	// 衝突離脱(OnCollisionExit)
	for (const auto& oldPair : previousCollisions_) {
		// 前フレームにあったペアが今フレームに無い場合は離脱
		if (currentCollisions.find(oldPair) == currentCollisions.end()) {
			auto [idA, idB] = oldPair;
			BaseCollider3D* colliderA = FindColliderByID(activeColliders_, idA);
			BaseCollider3D* colliderB = FindColliderByID(activeColliders_, idB);
			if (!colliderA || !colliderB) {
				continue;
			}
			colliderA->GetIsSellingOther() = false;
			colliderB->GetIsSellingOther() = false;

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

	// 今回フレームの衝突結果リストを保存
	previousCollisions_ = currentCollisions;
}

void CollisionManager::Clear() {
	activeColliders_.clear();
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
	// SphereColliderにキャスト
	SphereCollider* sphereA = static_cast<SphereCollider*>(colliderA);
	SphereCollider* sphereB = static_cast<SphereCollider*>(colliderB);
	assert(sphereA && sphereB && "Colliders must be SphereCollider!");

	// 中心座標を取得
	Vector3 centerA = sphereA->worldPosition_;
	Vector3 centerB = sphereB->worldPosition_;

	// 半径を取得
	float radiusA = sphereA->GetRadius();
	float radiusB = sphereB->GetRadius();

	// 二点間の距離判定
	Vector3 diff = centerA - centerB;
	float distance = Length(diff);
	float radiusSum = radiusA + radiusB;

	if (distance <= radiusSum) {
		return true;  // 衝突している
	}
	return false;     // 衝突していない
}
