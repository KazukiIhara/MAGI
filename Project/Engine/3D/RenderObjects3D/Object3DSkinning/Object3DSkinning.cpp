#include "Object3DSkinning.h"

#include "Framework/MAGI.h"


Object3DSkinning::Object3DSkinning(const std::string& objectName, const std::string& modelName)
	:Object3D(objectName, modelName) {}

Object3DSkinning::~Object3DSkinning() {

}

void Object3DSkinning::Initialize() {
	// モデル取得
	model_ = std::make_unique<SkinningModel>(modelData_);
	model_->Initialize();
	assert(model_ && "Warning: Not found model");
}

void Object3DSkinning::Update() {
	// アニメーション再生中なら
	if (currentAnimationName_ != "") {
		animationTime_ += MAGISYSTEM::GetDeltaTime();
		animationTime_ = std::fmod(animationTime_, currentAnimation_.duration);

		if (auto skinmodel = static_cast<SkinningModel*>(model_.get())) {
			skinmodel->ApplyAnimation(currentAnimation_, animationTime_);
		}
	}
	// 基底クラスの更新
	Object3D::Update();
}

void Object3DSkinning::PlayAnimation(const std::string& animationName) {
	// 名前をセット
	currentAnimationName_ = animationName;
	// アニメーションを持ってくる
	currentAnimation_ = MAGISYSTEM::FindAnimation(currentAnimationName_);
	// タイマーリセット
	animationTime_ = 0.0f;
}

void Object3DSkinning::ResetAnimation() {
	// 名前を削除
	currentAnimationName_ = "";
	// アニメーションをリセット
	if (auto skinmodel = static_cast<SkinningModel*>(model_.get())) {
		skinmodel->ApplyAnimation(currentAnimation_, 0.0f);
	}
	currentAnimation_ = AnimationData{};
	// タイマーリセット
	animationTime_ = 0.0f;
}

void Object3DSkinning::SwitchAnimation(const std::string& animationName) {
	animationName;
	// TODO::アニメーションの切り替え部分を補間する
}

void Object3DSkinning::DrawSkeleton() {

	if (auto skinModel = static_cast<SkinningModel*>(model_.get())) {
		skinModel->DrawSkeleton();
	}

}
