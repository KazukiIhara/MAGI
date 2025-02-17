#include "ColliderManager.h"
#include "Logger/Logger.h"

#include <cassert>

ColliderManager::ColliderManager() {
	colliders_.clear();
	Logger::Log("ColliderManager Initialize\n");
}

ColliderManager::~ColliderManager() {
	Logger::Log("ColliderManager Finalize\n");
}

void ColliderManager::Update() {
	for (auto& collider : colliders_) {
		if (collider) {
			collider->Update();
			collider->TransferShape();
		}
	}
}

void ColliderManager::Create(const std::string& name, Collider3DType colliderType) {
	std::unique_ptr<BaseCollider3D> newCollider;
	switch (colliderType) {
	case Collider3DType::Sphere:
		newCollider = std::make_unique<SphereCollider>(name, currentID_, colliderType);
		break;
	case Collider3DType::AABB:
		break;
	case Collider3DType::OBB:
		break;
	}
	// コンテナに登録
	colliders_.push_back(std::move(newCollider));
	// 識別IDをインクリメント
	currentID_++;
}

void ColliderManager::Remove(const std::string& name) {
	// ベクターを走査して、名前が一致するコライダーを探す
	for (auto it = colliders_.begin(); it != colliders_.end(); ++it) {
		// (*it)->name_ でコライダー名を取得
		if ((*it)->name_ == name) {
			colliders_.erase(it);
			return;  // 見つかったら削除して関数を抜ける
		}
	}
	// 見つからなかった場合
	assert(false && "Not Found Collider to Remove");
}

BaseCollider3D* ColliderManager::Find(const std::string& name) {
	// ベクターを走査して、名前が一致するコライダーを探す
	for (auto& collider : colliders_) {
		if (collider && collider->name_ == name) {
			return collider.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found Collider");
	return nullptr;
}

void ColliderManager::Clear() {
	colliders_.clear();
	currentID_ = 0;
}

const std::vector<std::unique_ptr<BaseCollider3D>>& ColliderManager::GetColliders() const {
	return colliders_;
}