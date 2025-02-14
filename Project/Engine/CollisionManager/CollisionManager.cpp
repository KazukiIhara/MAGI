#include "CollisionManager.h"

#include <cassert>

CollisionManager::CollisionManager() {
	//
	// 各コライダーの組み合わせに対応する衝突判定関数を登録する
	//

	// 球体同士
	collisionFuncMap_[{Collider3DType::Sphere, Collider3DType::Sphere}] = CheckSphereToSphereCollision;
}

void CollisionManager::Update() {

}

void CollisionManager::Add(BaseCollider3D* baseCollider3D) {

}

void CollisionManager::CheckCollisionPair(BaseCollider3D* colliderA, BaseCollider3D* colliderB) {

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
	return true;
}
