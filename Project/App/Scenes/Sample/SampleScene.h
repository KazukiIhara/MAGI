#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/RenderObjects3D/Object3D/Object3D.h"
#include "3D/RenderObjects3D/Object3DSkinning/Object3DSkinning.h"
#include "3D/RenderObjects3D/Primitive3D/Primitive3D.h"
#include "2D/Object2D/Object2D.h"

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
	std::unique_ptr<Object2D> object2DSample_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadTexture("pronama_chan.png");
	MAGISYSTEM::LoadModel("Kick");
	MAGISYSTEM::LoadModel("terrain", true);
	MAGISYSTEM::LoadModel("Man");
	MAGISYSTEM::LoadModel("Maw");

	MAGISYSTEM::LoadAnimation("Reaction", false);
	MAGISYSTEM::LoadAnimation("Kick");
	MAGISYSTEM::LoadAnimation("Maw");

	primitive_ = std::make_unique<Primitive3D>("Plane", Primitive3DType::Plane);
	primitive_->GetRotate().y = std::numbers::pi_v<float>;
	primitive_->GetTranslate().y = 1.0f;

	terrain_ = std::make_unique<Object3D>("terrain", "terrain");
	terrain_->Initialize();

	skinningSample_ = std::make_unique<Object3DSkinning>("Kick", "Maw");
	skinningSample_->Initialize();
	//skinningSample_->GetTranslate().x = -1.0f;

	sample_ = std::make_unique<Object3DSkinning>("Kick", "Kick");
	sample_->Initialize();
	sample_->GetTranslate().x = 1.0f;
	sample_->GetRotate().y = std::numbers::pi_v<float>;

	object2DSample_ = std::make_unique<Object2D>("pronama", "pronama_chan.png");


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
		skinningSample_->PlayAnimation("Capoeira");
	}

	if (MAGISYSTEM::TriggerKey(DIK_0)) {
		sample_->ResetAnimation();
		skinningSample_->ResetAnimation();
	}

	primitive_->Update();

	terrain_->Update();

	skinningSample_->Update();

	sample_->Update();

	object2DSample_->Update();

	MAGISYSTEM::DrawLine3D(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Color::Red);

}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	// 
	// 法線マップなしオブジェクト3Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject3D();
	//primitive_->Draw();
	//sample_->Draw();

	// 
	// 法線マップありオブジェクト3Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject3DNormalMap();
	skinningSample_->Draw();
	terrain_->Draw();

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();
	//object2DSample_->Draw();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
