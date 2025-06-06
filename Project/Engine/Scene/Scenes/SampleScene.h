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
class SampleScene: public BaseScene<Data> {
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

	// トランスフォーム
	std::unique_ptr<Transform3D> transform_;
	std::unique_ptr<Transform3D> transformPlane_;

	std::array<std::unique_ptr<Transform3D>, 30000> transforms_;

	// 板ポリ描画用の頂点データ
	PlaneData3D planeData_{};

	// 三角形描画用の頂点データ
	TriangleData3D triangleData_{};

	// ボックス描画用の頂点データ
	BoxData3D boxData_{};

	// 球体描画用の頂点データ
	SphereData3D sphereData_{};

	// リング描画用の頂点データ
	RingData3D ringData_{};

	// シリンダー描画用の頂点データ
	CylinderData3D cylinderData_{};

	// プリミティブ描画用のマテリアルデータ
	PrimitiveMaterialData3D material_{};

	// スプライト用のマテリアルデータ
	SpriteMaterialData spriteMaterial{};

	// モデル用のマテリアルデータ
	ModelMaterial modelMaterial_{};

	// モデル用のマテリアルデータ
	ModelMaterial modelMatAlpha_{};

	float vignetteScale_ = 16.0f;
	float vignetteFalloff_ = 0.8f;

	float gaussianSigma_ = 0.5f;

	Vector2 radialBlurCenter_ = { 0.5f,0.5f };
	float radialBlurWidth_ = 0.01f;

	bool isOnGrayscale_ = false;
	bool isOnGaussian_ = false;
	bool isOnVignette_ = false;
	bool isRadialBlur_ = false;

	static const uint32_t wtsNum_ = 3;

	// DirectionalLight
	DirectionalLight directionalLight_{};

	// スプライトデータ
	SpriteData spriteData_{};

	std::unique_ptr<SimpleAnimation<Vector3>> simpleAnimation_;

	float t_ = 0.0f;

	float yaw_ = 0.0f;
	float pitch_ = 0.0f;
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

	// モデルのマテリアル設定
	modelMaterial_.blendMode = BlendMode::None;

	modelMatAlpha_.blendMode = BlendMode::Add;

	spriteData_.isBack = true;

	// スプライト用のマテリアルデータ
	spriteMaterial.blendmode = BlendMode::Normal;
	spriteMaterial.color = Vector4(1.0f, 1.0f, 1.0f, 0.5f);
	spriteMaterial.textureName = "pronama_chan.png";
	spriteMaterial.isFlipX = false;

	// ModelDrawer
	MAGISYSTEM::CreateModelDrawer("test", MAGISYSTEM::FindModel("teapot"));

	// マテリアルを設定
	material_.blendMode = BlendMode::None;

	planeData_.verticesOffsets[Plane3DVertices::LeftTop] = { -10.0f,10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightTop] = { 10.0f,10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::LeftBottom] = { -10.0f,-10.0f,0.0f };
	planeData_.verticesOffsets[Plane3DVertices::RightBottom] = { 10.0f,-10.0f,0.0f };

	transform_ = std::make_unique<Transform3D>();
	transformPlane_ = std::make_unique<Transform3D>(Vector3(1.0f, 1.0f, 1.0f), Vector3(std::numbers::pi_v<float>*0.5f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f));

	for (uint32_t i = 0; i < 30000; i++) {
		transforms_[i] = std::make_unique<Transform3D>(Vector3(0.0f, 0.0f, float(i)));
	}

	// アニメーション作成
	simpleAnimation_ = std::make_unique<SimpleAnimation<Vector3>>(
		Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
		EasingType::EaseInOutSine, true, LoopType::PingPong
	);
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("Translate");
	Vector3 tempScale = transform_->GetScale();
	if (ImGui::DragFloat3("Scale", &tempScale.x, 0.01f)) {
		transform_->SetScale(tempScale);
	}
	Vector3 tempRotate = transform_->GetRotate();
	if (ImGui::DragFloat3("Rotate", &tempRotate.x, 0.01f)) {
		transform_->SetRotate(tempRotate);
	}
	Vector3 tempTranslate = transform_->GetTranslate();
	if (ImGui::DragFloat3("Translate", &tempTranslate.x, 0.01f)) {
		transform_->SetTranslate(tempTranslate);
	}
	ImGui::End();

	ImGui::Begin("PlaneData");
	ImGui::DragFloat3("LeftTop", &planeData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &planeData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &planeData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &planeData_.verticesOffsets[3].x, 0.01f);
	ImGui::End();

	ImGui::Begin("BoxData");
	ImGui::DragFloat3("LeftTopFront", &boxData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTopFront", &boxData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottomFront", &boxData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottomFront", &boxData_.verticesOffsets[3].x, 0.01f);
	ImGui::DragFloat3("LeftTopBack", &boxData_.verticesOffsets[4].x, 0.01f);
	ImGui::DragFloat3("RightTopBack", &boxData_.verticesOffsets[5].x, 0.01f);
	ImGui::DragFloat3("LeftBottomBack", &boxData_.verticesOffsets[6].x, 0.01f);
	ImGui::DragFloat3("RightBottomBack", &boxData_.verticesOffsets[7].x, 0.01f);
	ImGui::End();

	ImGui::Begin("SphereData");
	int tempH = sphereData_.horizontalSegments;
	ImGui::DragInt("HorizontalSegments", &tempH);
	sphereData_.horizontalSegments = tempH;
	int tempV = sphereData_.verticalSegments;
	ImGui::DragInt("VerticalSegments", &tempV);
	sphereData_.verticalSegments = tempV;
	ImGui::DragFloat("Radius", &sphereData_.radius);
	ImGui::End();

	ImGui::Begin("RingData");
	int ringDivideTemp = ringData_.ringDivide;
	ImGui::DragInt("Divide", &ringDivideTemp);
	ringData_.ringDivide = ringDivideTemp;
	ImGui::DragFloat("InnerRadius", &ringData_.innerRadius, 0.01f);
	ImGui::DragFloat("OuterRadius", &ringData_.outerRadius, 0.01f);
	ImGui::End();

	ImGui::Begin("CylinderData");
	int cylinderDivideTemp = cylinderData_.divide;
	ImGui::DragInt("Divide", &cylinderDivideTemp);
	cylinderData_.divide = cylinderDivideTemp;
	ImGui::DragFloat("TopRadius", &cylinderData_.topRadius, 0.01f);
	ImGui::DragFloat("BottomRadius", &cylinderData_.bottomRadius, 0.01f);
	ImGui::DragFloat("Height", &cylinderData_.height, 0.01f);
	ImGui::End();

	ImGui::Begin("Material");
	ImGui::DragFloat2("uvScale", &material_.uvScale.x, 0.01f);
	ImGui::DragFloat("uvRotate", &material_.uvRotate, 0.01f);
	ImGui::DragFloat2("uvTranslate", &material_.uvTranslate.x, 0.01f);
	ImGui::ColorEdit4("Color", &material_.baseColor.x);
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

	ImGui::Begin("Sound");
	if (ImGui::Button("Coin")) {
		MAGISYSTEM::PlayWaveSound("coin.wav");
	}
	ImGui::End();

	ImGui::Begin("DirectionalLight");
	ImGui::DragFloat3("Direction", &directionalLight_.direction.x, 0.01f);
	directionalLight_.direction = MAGIMath::Normalize(directionalLight_.direction);
	ImGui::DragFloat("Intensity", &directionalLight_.intensity, 0.01f);
	ImGui::ColorEdit3("Color", &directionalLight_.color.x);
	ImGui::End();

	ImGui::Begin("SpriteData");
	ImGui::DragFloat2("Position", &spriteData_.position.x, 1.0f);
	ImGui::Checkbox("IsFlipX", &spriteMaterial.isFlipX);
	ImGui::Checkbox("IsFlipY", &spriteMaterial.isFlipY);
	ImGui::End();

	ImGui::Begin("SimpleAnimation");
	ImGui::DragFloat("t", &t_, 0.01f);
	ImGui::End();

	ImGui::Begin("SceneCamera");
	ImGui::DragFloat("Yaw", &yaw_, 0.01f);
	ImGui::DragFloat("Pitch", &pitch_, 0.01f);
	ImGui::End();

	sceneCamera_->SetYaw(yaw_);
	sceneCamera_->SetPitch(pitch_);

	t_ += MAGISYSTEM::GetDeltaTime();

	//transform_->SetTranslate(simpleAnimation_->GetValue(t_));

	transform_->Update();
	transformPlane_->Update();

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
	// 球体描画
	for (uint32_t i = 0; i < 10000; i++) {
		MAGISYSTEM::DrawModel("test", transforms_[i]->GetWorldMatrix(), modelMaterial_);
	}
	MAGISYSTEM::DrawPlane3D(transformPlane_->GetWorldMatrix(), planeData_, material_);
}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
