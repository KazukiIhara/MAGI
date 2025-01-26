#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/Object3D/Object3D.h"
#include "3D/Object3DSkinning/Object3DSkinning.h"
#include "3D/Primitive3D/Primitive3D.h"

// サンプルシーン
template <typename Data>
class SampleScene: public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~SampleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	std::unique_ptr<Object3D> sample_ = nullptr;
	std::unique_ptr<Object3DSkinning> skinningSample_ = nullptr;
	std::unique_ptr<Object3D> terrain_ = nullptr;
	std::unique_ptr<Primitive3D> primitive_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("boxMan");
	MAGISYSTEM::LoadModel("crossMan");
	MAGISYSTEM::LoadModel("terrain", true);
	MAGISYSTEM::LoadModel("teapot", true);

	MAGISYSTEM::LoadAnimation("boxMan");

	//sample_ = std::make_unique<Object3D>("sample", "teapot");
	primitive_ = std::make_unique<Primitive3D>("primitive", Primitive3DType::Sphere);


	terrain_ = std::make_unique<Object3D>("terrain", "terrain");
	terrain_->Initialize();

	skinningSample_ = std::make_unique<Object3DSkinning>("skinningSample", "boxMan");
	skinningSample_->Initialize();

	MAGISYSTEM::AddPunctualLight("sampleLight");

	auto& sampleLight = MAGISYSTEM::GetLightData("sampleLight");
	sampleLight.intensity = 1.0f;


	//MAGISYSTEM::AddPunctualLight("redLight");
	//auto& redLightData = MAGISYSTEM::GetLightData("redLight");
	//redLightData.type = 1;
	//redLightData.color = { 1.0f,0.0f,0.0f };
	//redLightData.intensity = 7.0f;
	//redLightData.position = { 3.0f,2.0f,0.0f };


	//MAGISYSTEM::AddPunctualLight("blueLight");
	//auto& blueLightData = MAGISYSTEM::GetLightData("blueLight");
	//blueLightData.type = 1;
	//blueLightData.color = { 0.0f,0.0f,1.0f };
	//blueLightData.intensity = 7.0f;
	//blueLightData.position = { -3.0f,2.0f,0.0f };
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	terrain_->Update();

	skinningSample_->Update();

	if (MAGISYSTEM::TriggerKey(DIK_1)) {
		skinningSample_->PlayAnimation("Action.001");
	}
	if (MAGISYSTEM::TriggerKey(DIK_2)) {
		skinningSample_->PlayAnimation("Action.002");
	}
	if (MAGISYSTEM::TriggerKey(DIK_3)) {
		skinningSample_->ResetAnimation();
	}

	//sample_->Update();
	primitive_->Update();
}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	MAGISYSTEM::PreDrawObject3DNormalMap();
	//terrain_->Draw();

	MAGISYSTEM::PreDrawObject3D();
	//skinningSample_->Draw();

	//sample_->Draw();
	primitive_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
