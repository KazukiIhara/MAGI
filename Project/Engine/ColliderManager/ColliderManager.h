#pragma once

#include <map>
#include <string>

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"
#include "3D/Colliders3D/SphereCollider/SphereCollider.h"

/// <summary>
/// コライダーマネージャ
/// </summary>
class ColliderManager {
public:
	ColliderManager();
	~ColliderManager();

	void Update();

	void Create(const std::string& name, Collider3DType colliderType);
	void Remove(const std::string& name);

	BaseCollider3D* Find(const std::string& name);

	void Clear();
	
private:
	// コライダーコンテナ
	std::map<std::string, std::unique_ptr<BaseCollider3D>> colliders_;
	// コライダーID
	uint32_t currentID_ = 0;
};