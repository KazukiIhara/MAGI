#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include "2D/Object2D/Object2D.h"

using namespace MAGIUtility;

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
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_[5]{};

	// 板ポリ描画用の頂点データ
	PlaneData3D planeData_{};

	// 三角形描画用の頂点データ
	TriangleData3D triangleData_{};

	// 球体描画用の頂点データ
	SphereData3D sphereData_{};

	// リング描画用の頂点データ
	RingData3D ringData_{};

	// シリンダー描画用の頂点データ
	CylinderData3D cylinderData_{};

	// プリミティブ描画用のマテリアルデータ
	PrimitiveMaterialData3D material_{};

	// モデル用のマテリアルデータ
	ModelMaterial modelMaterial_{};

	float vignetteScale_ = 16.0f;
	float vignetteFalloff_ = 0.8f;
	float gaussianSigma_ = 0.5f;

	static const uint32_t wtsNum_ = 40000;

	std::array<WorldTransform, wtsNum_> wts_;


	// DirectionalLight
	DirectionalLight directionalLight_{};

};

template<typename Data>
inline void SampleScene<Data>::Initialize() {

	//
	// アセットのロード
	//

	// テクスチャ
	MAGISYSTEM::LoadTexture("pronama_chan.png");
	// テクスチャ
	MAGISYSTEM::LoadTexture("gradationLine.png");

	// モデル
	MAGISYSTEM::LoadModel("terrain");
	MAGISYSTEM::LoadModel("teapot");
	MAGISYSTEM::LoadModel("Suzanne");

	// サウンド
	MAGISYSTEM::LoadWaveSound("Alarm01.wav");
	MAGISYSTEM::LoadWaveSound("coin.wav");

	//
	// オブジェクトの作成
	//

	// カメラ

	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");

	// マネージャに追加
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));

	// カメラの設定
	MAGISYSTEM::SetCurrentCamera("SceneCamera");

	// ライト
	MAGISYSTEM::AddPunctualLight("SampleLight");


	modelMaterial_.blendMode = BlendMode::None;

	// ModelDrawer
	MAGISYSTEM::CreateModelDrawer("test", MAGISYSTEM::FindModel("teapot"));

	// トランスフォーム初期化
	for (uint32_t i = 0; i < 5; i++) {
		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translate_.x = -4.0f;
	worldTransform_[1].translate_.x = -2.0f;
	worldTransform_[2].translate_.x = 0.0f;
	worldTransform_[3].translate_.x = 2.0f;
	worldTransform_[4].translate_.x = 4.0f;

	// デフォルトのテクスチャを取得　TODO:マテリアルもクラス化して初期化できるようにする
	material_.textureIndex = MAGISYSTEM::GetDefaultTextureIndex();
	material_.blendMode = BlendMode::None;

	for (uint32_t i = 0; i < wtsNum_; i++) {
		wts_[i].Initialize();
		wts_[i].translate_.z = float(i);
	}

}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("Translate0");
	ImGui::DragFloat3("Scale", &worldTransform_[0].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[0].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[0].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate1");
	ImGui::DragFloat3("Scale", &worldTransform_[1].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[1].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[1].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate2");
	ImGui::DragFloat3("Scale", &worldTransform_[2].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[2].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[2].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Translate3");
	ImGui::DragFloat3("Scale", &worldTransform_[3].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[3].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[3].translate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("PlaneData");
	ImGui::DragFloat3("LeftTop", &planeData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &planeData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &planeData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &planeData_.verticesOffsets[3].x, 0.01f);
	ImGui::End();

	ImGui::Begin("Material");
	ImGui::DragFloat2("uvScale", &material_.uvScale.x, 0.01f);
	ImGui::DragFloat("uvRotate", &material_.uvRotate, 0.01f);
	ImGui::DragFloat2("uvTranslate", &material_.uvTranslate.x, 0.01f);
	ImGui::ColorEdit4("Color", &material_.baseColor.x);
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


	ImGui::Begin("VignetteParamater");
	ImGui::DragFloat("Scale", &vignetteScale_, 0.01f);
	ImGui::DragFloat("Falloff", &vignetteFalloff_, 0.01f);
	ImGui::End();

	ImGui::Begin("GaussianBlurParamater");
	ImGui::DragFloat("Sigma", &gaussianSigma_, 0.01f);
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

	// トランスフォーム更新
	for (uint32_t i = 0; i < 5; i++) {
		worldTransform_[i].Update();
	}

	for (uint32_t i = 0; i < wtsNum_; i++) {
		wts_[i].Update();
	}

	MAGISYSTEM::SetDirectionalLight(directionalLight_);

	// ポストエフェクトをかける
	//MAGISYSTEM::ApplyPostEffectGrayScale();
	//MAGISYSTEM::ApplyPostEffectGaussianX(gaussianSigma_, 13);
	//MAGISYSTEM::ApplyPostEffectGaussianY(gaussianSigma_, 13);
	//MAGISYSTEM::ApplyPostEffectVignette(vignetteScale_, vignetteFalloff_);
}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(worldTransform_[0].worldMatrix_, planeData_, material_);

	//// 球体描画
	//MAGISYSTEM::DrawSphere3D(worldTransform_[1].worldMatrix_, sphereData_, material_);

	//// 三角形描画
	//MAGISYSTEM::DrawTriangle3D(worldTransform_[2].worldMatrix_, triangleData_, material_);

	//// モデル描画
	//MAGISYSTEM::DrawModel("test", worldTransform_[2].worldMatrix_, modelMaterial_);

	// モデル描画


	for (uint32_t i = 0; i < wtsNum_; i++) {
		MAGISYSTEM::DrawModel("test", wts_[i].worldMatrix_, modelMaterial_);
	}

	MAGISYSTEM::DrawLine3D(Vector3(0.0f, -2.0f, 0.0f), Vector3(0.0f, 2.0f, 0.0f), Color::Crimson);

	// リング描画
	// MAGISYSTEM::DrawRing3D(worldTransform_[3].worldMatrix_, ringData_, material_);

	//// シリンダー描画
	//MAGISYSTEM::DrawCylinder3D(worldTransform_[4].worldMatrix_, cylinderData_, material_);

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
