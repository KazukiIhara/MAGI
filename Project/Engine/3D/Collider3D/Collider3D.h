#pragma once

#include <memory>

#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

class GameObject3D;

class Collider3D:public WorldEntity {
public:
	Collider3D(GameObject3D* owner);
	~Collider3D();

private:
	// 所属するゲームオブジェクトのポインタ
	GameObject3D* owner_ = nullptr;
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

};