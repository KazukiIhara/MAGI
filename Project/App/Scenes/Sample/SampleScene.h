#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/RenderObjects3D/Object3D/Object3D.h"
#include "3D/RenderObjects3D/Object3DSkinning/Object3DSkinning.h"
#include "3D/RenderObjects3D/Primitive3D/Primitive3D.h"

// サンプルシーン
template <typename Data>
class SampleScene : public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~SampleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	std::unique_ptr<Object3DSkinning> sample_ = nullptr;
	std::unique_ptr<Object3DSkinning> skinningSample_ = nullptr;
	std::unique_ptr<Object3D> terrain_ = nullptr;
	std::unique_ptr<Primitive3D> primitive_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadTexture("pronama_chan.png");
	MAGISYSTEM::LoadModel("Kick");
	MAGISYSTEM::LoadModel("XBot");
	MAGISYSTEM::LoadModel("terrain", true);
	MAGISYSTEM::LoadModel("teapot", true);
	MAGISYSTEM::LoadModel("walk");
	MAGISYSTEM::LoadModel("Man");


	MAGISYSTEM::LoadAnimation("walk");
	MAGISYSTEM::LoadAnimation("sneakWalk");
	MAGISYSTEM::LoadAnimation("Kick");
	MAGISYSTEM::LoadAnimation("Reaction", false);


	primitive_ = std::make_unique<Primitive3D>("primitive", Primitive3DType::Sphere);
	primitive_->GetTranslate().y = 1.0f;
	primitive_->GetMaterial().enableSpecularRef = true;

	terrain_ = std::make_unique<Object3D>("terrain", "terrain");
	terrain_->Initialize();

	skinningSample_ = std::make_unique<Object3DSkinning>("Kick", "Man");
	skinningSample_->Initialize();
	skinningSample_->GetTranslate().x = -1.0f;

	sample_ = std::make_unique<Object3DSkinning>("Kick", "Kick");
	sample_->Initialize();
	sample_->GetTranslate().x = 1.0f;
	

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
	if (MAGISYSTEM::TriggerKey(DIK_1)) {
		sample_->PlayAnimation("Kick");
	}

	if (MAGISYSTEM::TriggerKey(DIK_3)) {
		skinningSample_->PlayAnimation("Reaction");
	}

	if (MAGISYSTEM::TriggerKey(DIK_0)) {
		skinningSample_->ResetAnimation();
	}

	terrain_->Update();

	primitive_->Update();

	skinningSample_->Update();
	sample_->Update();

}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	MAGISYSTEM::PreDrawObject3DNormalMap();
	terrain_->Draw();

	MAGISYSTEM::PreDrawObject3D();
	//primitive_->Draw();
	skinningSample_->Draw();
	sample_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
