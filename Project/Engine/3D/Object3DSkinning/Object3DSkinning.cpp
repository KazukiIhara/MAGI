#include "Object3DSkinning.h"

Object3DSkinning::Object3DSkinning(const std::string& objectName, const std::string& modelName)
	:Object3D(objectName, modelName) {
	
}

void Object3DSkinning::Update() {
	// 基底クラスの更新
	Object3D::Update();

	// スケルトンの更新
	skeleton_->Update();
}
