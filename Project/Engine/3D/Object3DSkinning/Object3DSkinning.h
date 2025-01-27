#pragma once

#include "3D/Object3D/Object3D.h"
#include "3D/SkinningModel/SkinningModel.h"

/// <summary>
/// スキニングつき3Dオブジェクト(インスタンスの生成後初期化メソッドを呼んでください)
/// </summary>
class Object3DSkinning:public Object3D {
public:
	Object3DSkinning(const std::string& objectName, const std::string& modelName);
	~Object3DSkinning()override;
	void Initialize();
	void Update()override;

	void PlayAnimation(const std::string& animationName);
	void ResetAnimation();
	void SwitchAnimation(const std::string& animationName);
private:
	// アニメーション用変数
	float animationTime_ = 0.0f;
	// 現在再生中のアニメーション名
	std::string currentAnimationName_ = "";
	// 現在再生中のアニメーション
	AnimationData currentAnimation_{};
};