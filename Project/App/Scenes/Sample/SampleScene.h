#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/Object3D/Object3D.h"
#include "3D/Camera3D/Camera3D.h"

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
	std::unique_ptr<Camera3D> camera_ = nullptr;
	std::unique_ptr<Object3D> teapot_ = nullptr;
	std::unique_ptr<Object3D> ground_ = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::LoadModel("Ground");

	camera_ = std::make_unique<Camera3D>();
	teapot_ = std::make_unique<Object3D>("teapot");
	ground_ = std::make_unique<Object3D>("Ground");

	teapot_->GetTranslate().y = 0.5f;

	MAGISYSTEM::AddPunctualLight("sampleLight");
	MAGISYSTEM::AddPunctualLight("redLight");

	auto& sampleLight = MAGISYSTEM::GetLightData("sampleLight");
	sampleLight.intensity = 0.1f;

	auto& redLightData = MAGISYSTEM::GetLightData("redLight");
	redLightData.type = 2;
	redLightData.color = { 1.0f,0.0f,0.0f };
	redLightData.position = { 0.0f,2.0f,0.0f };
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	if (MAGISYSTEM::TriggerKey(DIK_R)) {
		teapot_->GetRotate() = { 0.0f,0.0f,0.0f };
	}

	if (MAGISYSTEM::PushKey(DIK_W)) {
		teapot_->GetRotate().x += 0.01f;
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
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	ground_->Draw();
	teapot_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
