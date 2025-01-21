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
	std::unique_ptr<Object3D> object3d_ = nullptr;
	PunctualLightData light_{};
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("teapot");

	camera_ = std::make_unique<Camera3D>();
	object3d_ = std::make_unique<Object3D>("teapot");
	object3d_->SetCamera(camera_.get());

	light_.type = static_cast<uint32_t>(PunctualLightType::Directional);
	light_.color = { 1.0f,1.0f,1.0f };

	MAGISYSTEM::AddPunctualLight("sampleLight", light_);
}

template<typename Data>
inline void SampleScene<Data>::Update() {


	if (MAGISYSTEM::TriggerKey(DIK_R)) {
		object3d_->GetRotate() = { 0.0f,0.0f,0.0f };
	}

	if (MAGISYSTEM::PushKey(DIK_W)) {
		object3d_->GetRotate().x += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_S)) {
		object3d_->GetRotate().x -= 0.01f;
	}

	if (MAGISYSTEM::PushKey(DIK_D)) {
		object3d_->GetRotate().y += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_A)) {
		object3d_->GetRotate().y -= 0.01f;
	}

	if (MAGISYSTEM::PushKey(DIK_Q)) {
		object3d_->GetRotate().z += 0.01f;
	} else if (MAGISYSTEM::PushKey(DIK_E)) {
		object3d_->GetRotate().z -= 0.01f;
	}

	camera_->Update();
	object3d_->Update();
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	object3d_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
