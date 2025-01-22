#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/Object3D/Object3D.h"
#include "3D/Primitive3D/Primitive3D.h"

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
	std::unique_ptr<Object3D> teapot_ = nullptr;
	std::unique_ptr<Object3D> ground_ = nullptr;
	std::unique_ptr<Primitive3D> plane_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::LoadModel("terrain");

	teapot_ = std::make_unique<Object3D>("teapot", "teapot");
	ground_ = std::make_unique<Object3D>("terrain", "terrain");
	plane_ = std::make_unique<Primitive3D>("plane", Primitive3DType::Plane);

	teapot_->GetTranslate().y = 0.8f;

	MAGISYSTEM::AddPunctualLight("sampleLight");

	auto& sampleLight = MAGISYSTEM::GetLightData("sampleLight");
	sampleLight.intensity = 1.0f;


	//MAGISYSTEM::AddPunctualLight("redLight");
	//auto& redLightData = MAGISYSTEM::GetLightData("redLight");
	//redLightData.type = 1;
	//redLightData.color = { 1.0f,0.0f,0.0f };
	//redLightData.intensity = 7.0f;
	//redLightData.position = { 2.0f,2.0f,0.0f };


	//MAGISYSTEM::AddPunctualLight("blueLight");
	//auto& blueLightData = MAGISYSTEM::GetLightData("blueLight");
	//blueLightData.type = 1;
	//blueLightData.color = { 0.0f,0.0f,1.0f };
	//blueLightData.intensity = 7.0f;
	//blueLightData.position = { -2.0f,2.0f,0.0f };
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	if (MAGISYSTEM::TriggerKey(DIK_R)) {
		teapot_->GetRotate() = { 0.0f,0.0f,0.0f };
	}

	if (MAGISYSTEM::PushKey(DIK_W)) {
		plane_->GetRotate().x += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_S)) {
		teapot_->GetRotate().x -= 0.01f;
	}

	if (MAGISYSTEM::PushKey(DIK_D)) {
		teapot_->GetRotate().y += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_A)) {
		teapot_->GetRotate().y -= 0.01f;
	}

	if (MAGISYSTEM::PushKey(DIK_Q)) {
		teapot_->GetRotate().z += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_E)) {
		teapot_->GetRotate().z -= 0.01f;
	}

	ground_->Update();
	teapot_->Update();
	plane_->Update();
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	ground_->Draw();
	//teapot_->Draw();
	plane_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
