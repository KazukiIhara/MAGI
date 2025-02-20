#pragma once

#include <vector>
#include <string>

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"
#include "3D/Colliders3D/SphereCollider/SphereCollider.h"
#include "3D/Colliders3D/AABBCollider3D/AABBCollider3D.h"

/// <summary>
/// コライダーマネージャ
/// </summary>
class ColliderManager {
public:
	ColliderManager();
	~ColliderManager();

	void Update();

	std::string Create(const std::string& name, Collider3DType colliderType);
	void Remove(const std::string& name);

	BaseCollider3D* Find(const std::string& name);

	void Clear();

	// 生成された全コライダーのリストを取得
	const std::vector<std::unique_ptr<BaseCollider3D>>& GetColliders() const;

private:
	// コライダーコンテナ
	std::vector<std::unique_ptr<BaseCollider3D>> colliders_;
	// コライダーID
	uint32_t currentID_ = 0;
};