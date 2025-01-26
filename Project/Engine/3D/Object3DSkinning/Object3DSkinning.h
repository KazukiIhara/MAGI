#pragma once

#include "3D/Object3D/Object3D.h"
#include "3D/Skeleton/Skeleton.h"

class Object3DSkinning:public Object3D {
public:
	Object3DSkinning(const std::string& objectName, const std::string& modelName);
	~Object3DSkinning()override;

	void Update()override;
private:
	// スケルトン
	std::unique_ptr<Skeleton> skeleton_ = nullptr;
};