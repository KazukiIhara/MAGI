#pragma once

#include "3D/Object3D/Object3D.h"
#include "3D/SkinningModel/SkinningModel.h"

class Object3DSkinning:public Object3D {
public:
	Object3DSkinning(const std::string& objectName, const std::string& modelName);
	~Object3DSkinning()override;
	void Initialize();
	void Update()override;

	SkinningModel* GetSkinningModel() const;

private:
	// アニメーション用変数
	float animationTime_ = 0.0f;
};