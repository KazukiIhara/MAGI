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
		if (collider.second) {
			collider.second->Update();
		}
	}
}

void ColliderManager::Draw() {
	for (auto& collider : colliders_) {
		if (collider.second) {
			collider.second->Draw();
		}
	}
}

void ColliderManager::Create(const std::string& name, Collider3DType colliderType) {
	std::unique_ptr<BaseCollider3D> newCollider;

	switch (colliderType) {
		case Collider3DType::Sphere:
			newCollider = std::make_unique<SphereCollider>(colliderType);
			break;
		case Collider3DType::AABB:
			break;
		case Collider3DType::OBB:
			break;
	}

	// コンテナに登録
	colliders_.insert(std::pair(name, std::move(newCollider)));
}

void ColliderManager::Remove(const std::string& name) {
	// 作成済みコライダーを検索
	auto it = colliders_.find(name);
	if (it != colliders_.end()) {
		// コライダーが見つかった場合、削除する
		colliders_.erase(it);
	}
}

BaseCollider3D* ColliderManager::Find(const std::string& name) {
	// 作成済みコライダーを検索
	if (colliders_.contains(name)) {
		// コライダーを返す
		return colliders_.at(name).get();
	}

	// エラーメッセージ
	assert(false && "Not Found Collider");

	// ファイル名一致なし
	return nullptr;
}
