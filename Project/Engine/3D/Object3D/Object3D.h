#pragma once

#include "3D/WorldTransform/WorldTransform.h"


class Object3D {
public:
	Object3D();
	~Object3D();

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデルを受け取る箱
	
};