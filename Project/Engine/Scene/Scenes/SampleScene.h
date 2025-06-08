#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

using namespace MAGIUtility;

#include "Transform3D/Transform3D.h"

#include "SimpleAnimation/SimpleAnimation.h"

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
	// カメラ
	Camera3D* sceneCamera_ = nullptr;
	std::unique_ptr<Camera2D> sceneCamera2D_ = nullptr;
	float yaw_ = 0.0f;
	float pitch_ = 0.0f;

	// 
	// ポストエフェクト用変数
	// 

	float vignetteScale_ = 16.0f;
	float vignetteFalloff_ = 0.8f;

	float gaussianSigma_ = 0.5f;

	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;

	bool isOnGrayscale_ = false;
	bool isOnGaussian_ = false;
	bool isOnVignette_ = false;
	bool isRadialBlur_ = false;

	// DirectionalLight
	DirectionalLight directionalLight_{};


	Transform3D* transform0_;
	Transform3D* transform1_;

};

template<typename Data>
inline void SampleScene<Data>::Initialize() {

	//
	// アセットのロード
	//

	// テクスチャ
	MAGISYSTEM::LoadTexture("pronama_chan.png");
	MAGISYSTEM::LoadTexture("gradationLine.png");
	uint32_t skyBoxTexutreIndex = MAGISYSTEM::LoadTexture("kloppenheim_06_puresky_2k.dds");

	// モデル
	MAGISYSTEM::LoadModel("teapot");

	// サウンド
	MAGISYSTEM::LoadWaveSound("Alarm01.wav");
	MAGISYSTEM::LoadWaveSound("coin.wav");

	//
	// オブジェクトの作成
	//

	// カメラ

	// シーンカメラ作成
	std::unique_ptr<Camera3D> sceneCamera = std::make_unique<Camera3D>();
	// マネージャに追加
	MAGISYSTEM::AddCamera3D("SceneCamera", std::move(sceneCamera));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera3D("SceneCamera");
	sceneCamera_ = MAGISYSTEM::FindCamera3D("SceneCamera");

	// 2Dカメラ作成
	sceneCamera2D_ = std::make_unique<Camera2D>("SpriteCamera");
	// マネージャに追加
	MAGISYSTEM::AddCamera2D(std::move(sceneCamera2D_));
	// カメラを設定
	MAGISYSTEM::SetCurrentCamera2D("SpriteCamera");

	// スカイボックスの設定
	MAGISYSTEM::SetSkyBoxTextureIndex(skyBoxTexutreIndex);

	// ModelDrawer
	MAGISYSTEM::CreateModelDrawer("test", MAGISYSTEM::FindModel("teapot"));

	std::unique_ptr<Transform3D> transform0 = std::make_unique<Transform3D>();
	transform0_ = MAGISYSTEM::AddTransform3D(std::move(transform0));

	std::unique_ptr<Transform3D> transform1 = std::make_unique<Transform3D>(Vector3(2.0f, 0.0f, 0.0f));
	transform1_ = MAGISYSTEM::AddTransform3D(std::move(transform1));

	transform1_->SetParent(transform0_);

}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("ParentTest");

	Vector3 tempTranslate = transform0_->GetTranslate();
	if (ImGui::DragFloat3("Translate0", &tempTranslate.x, 0.01f)) {
		transform0_->SetTranslate(tempTranslate);
	}

	if (ImGui::Button("ParentKeepWorld")) {
		transform1_->SetParent(transform0_, true);
	}

	if (ImGui::Button("RemoveKeepWorld")) {
		transform1_->RemoveParent(true);
	}

	ImGui::End();

	ImGui::Begin("GrayscaleParamater");
	ImGui::Checkbox("On", &isOnGrayscale_);
	ImGui::End();

	ImGui::Begin("VignetteParamater");
	ImGui::Checkbox("On", &isOnVignette_);
	ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	ImGui::End();

	ImGui::Begin("GaussianBlurParamater");
	ImGui::Checkbox("On", &isOnGaussian_);
	ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
	ImGui::End();

	ImGui::Begin("RadialBlurParamater");
	ImGui::Checkbox("On", &isRadialBlur_);
	ImGui::DragFloat2("Center", &radialBlurCenter_.x, 0.01f);
	ImGui::DragFloat("BlurWidth", &radialBlurWidth_, 0.001f);
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	ImGui::DragFloat3("Direction", &directionalLight_.direction.x, 0.01f);
	directionalLight_.direction = MAGIMath::Normalize(directionalLight_.direction);
	ImGui::DragFloat("Intensity", &directionalLight_.intensity, 0.01f);
	ImGui::ColorEdit3("Color", &directionalLight_.color.x);
	ImGui::End();

	ImGui::Begin("SceneCamera");
	ImGui::DragFloat("Yaw", &yaw_, 0.01f);
	ImGui::DragFloat("Pitch", &pitch_, 0.01f);
	ImGui::End();

	sceneCamera_->SetYaw(yaw_);
	sceneCamera_->SetPitch(pitch_);

	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ポストエフェクトをかける
	if (isOnGrayscale_) {
		MAGISYSTEM::ApplyPostEffectGrayScale();
	}
	if (isOnGaussian_) {
		MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
		MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
	}
	if (isOnVignette_) {
		MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
	}
	if (isRadialBlur_) {
		MAGISYSTEM::ApplyPostEffectRadialBlur(radialBlurCenter_, radialBlurWidth_);
	}
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	MAGISYSTEM::DrawSphere3D(transform0_->GetWorldMatrix());
	MAGISYSTEM::DrawSphere3D(transform1_->GetWorldMatrix());
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
