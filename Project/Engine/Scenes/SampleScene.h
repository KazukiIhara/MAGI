#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"

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
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_[2]{};

	// 板ポリ描画用の頂点データ
	PlaneData3D planeData_[2]{};

	// 三角形描画用の頂点データ
	TriangleData3D triangleData_{};

	// 球体描画用の頂点データ
	SphereData3D sphereData_[2]{};

	// プリミティブ描画用のマテリアルデータ
	PrimitiveMaterialData3D material_{};

	static const uint32_t primitiveNum_ = 10000;

	WorldTransform worldTransform[primitiveNum_];
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {

	//
	// アセットのロード
	//

	// テクスチャ
	MAGISYSTEM::LoadTexture("pronama_chan.png");

	// モデル
	MAGISYSTEM::LoadModel("terrain");

	// サウンド
	MAGISYSTEM::LoadWaveSound("Alarm01.wav");

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

	// トランスフォーム初期化
	worldTransform_[0].Initialize();
	worldTransform_[1].Initialize();

	worldTransform_[0].translate_.x = -1.0f;
	worldTransform_[1].translate_.x = 1.0f;

	// デフォルトのテクスチャを取得　TODO:マテリアルもクラス化して初期化できるようにする
	material_.textureIndex = MAGISYSTEM::GetDefaultTextureIndex();

	for (size_t i = 0; i < primitiveNum_; i++) {
		worldTransform[i].Initialize();
		worldTransform[i].translate_.x = float(i);
	}

}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("Plane0");

	ImGui::DragFloat3("Scale", &worldTransform_[0].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[0].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[0].translate_.x, 0.01f);

	ImGui::DragFloat3("LeftTop", &planeData_[0].verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &planeData_[0].verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &planeData_[0].verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &planeData_[0].verticesOffsets[3].x, 0.01f);
	ImGui::End();


	ImGui::Begin("Sphere");

	ImGui::DragFloat3("Scale", &worldTransform_[0].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[0].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[0].translate_.x, 0.01f);

	ImGui::DragFloat("radius", &sphereData_[0].radius, 0.01f);

	int tempHS = sphereData_[0].horizontalSegments;
	ImGui::DragInt("horizontalSeg", &tempHS);
	sphereData_[0].horizontalSegments = tempHS;

	int tempVS = sphereData_[0].verticalSegments;
	ImGui::DragInt("verticalSeg", &tempVS);
	sphereData_[0].verticalSegments = tempVS;

	ImGui::End();

	ImGui::Begin("Sphere1");

	ImGui::DragFloat3("Scale", &worldTransform_[1].scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform_[1].rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &worldTransform_[1].translate_.x, 0.01f);

	ImGui::DragFloat("radius", &sphereData_[1].radius, 0.01f);

	int tempHS1 = sphereData_[1].horizontalSegments;
	ImGui::DragInt("horizontalSeg", &tempHS1);
	sphereData_[1].horizontalSegments = tempHS1;

	int tempVS1 = sphereData_[1].verticalSegments;
	ImGui::DragInt("verticalSeg", &tempVS1);
	sphereData_[1].verticalSegments = tempVS1;

	ImGui::End();

	// トランスフォーム更新
	for (uint32_t i = 0; i < 2; i++) {
		worldTransform_[i].Update();
	}


	for (size_t i = 0; i < primitiveNum_; i++) {
		worldTransform[i].Update();
	}

}

template<typename Data>
inline void SampleScene<Data>::Draw() {


	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(worldTransform_[0].worldMatrix_, planeData_[0], material_);


	MAGISYSTEM::DrawSphere3D(worldTransform_[1].worldMatrix_, sphereData_[0], material_);


	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
