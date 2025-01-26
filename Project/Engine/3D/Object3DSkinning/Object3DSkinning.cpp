#include "Object3DSkinning.h"

#include "Framework/MAGI.h"


Object3DSkinning::Object3DSkinning(const std::string& objectName, const std::string& modelName)
	:Object3D(objectName, modelName) {
	Initialize();
}

Object3DSkinning::~Object3DSkinning() {}

void Object3DSkinning::Initialize() {
	model_.reset();
	model_ = std::make_unique<SkinningModel>(modelData_);
	assert(model_ && "Warning: Not found model");
}

void Object3DSkinning::Update() {
	animationTime_ += MAGISYSTEM::GetDeltaTime();
	animationTime_ = std::fmod(animationTime_, MAGISYSTEM::FindAnimation("Action.001").duration);

	if (auto skinmodel = GetSkinningModel()) {
		skinmodel->ApplyAnimation(MAGISYSTEM::FindAnimation("Action.001"), animationTime_);
	}

	// 基底クラスの更新
	Object3D::Update();

}

SkinningModel* Object3DSkinning::GetSkinningModel() const {
	return dynamic_cast<SkinningModel*>(model_.get());
}
