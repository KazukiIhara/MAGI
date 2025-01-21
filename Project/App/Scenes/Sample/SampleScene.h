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
	std::unique_ptr<Object3D> sample_ = nullptr;
	PunctualLightData redLight_{};
	PunctualLightData blueLight_{};
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("terrain");

	camera_ = std::make_unique<Camera3D>();
	sample_ = std::make_unique<Object3D>("terrain");
	sample_->SetCamera(camera_.get());

	redLight_.type = static_cast<uint32_t>(PunctualLightType::Point);
	redLight_.color = { 1.0f,0.0f,0.0f };
	redLight_.position = { 0.5f,1.0f,0.0f };

	blueLight_.type = static_cast<uint32_t>(PunctualLightType::Point);
	blueLight_.color = { 0.0f,0.0f,1.0f };
	blueLight_.position = { -0.5f,1.0f,0.0f };


	MAGISYSTEM::AddPunctualLight("redLight", redLight_);
	MAGISYSTEM::AddPunctualLight("blueLight", blueLight_);
}

template<typename Data>
inline void SampleScene<Data>::Update() {
	camera_->Update();
	sample_->Update();
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	sample_->Draw();
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
