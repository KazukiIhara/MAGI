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
	WorldTransform worldTransform_{};

	// 板ポリ描画用の頂点データ
	PlaneData3D planeData_{};

	// 三角形描画用の頂点データ
	TriangleData3D triangleData_{};

	// プリミティブ描画用のマテリアルデータ
	PrimitiveMaterialData3D material_{};
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
	worldTransform_.Initialize();

	// デフォルトのテクスチャを取得　TODO:マテリアルもクラス化して初期化できるようにする
	material_.textureIndex = MAGISYSTEM::GetDefaultTextureIndex();
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	ImGui::Begin("Plane");
	ImGui::DragFloat3("LeftTop", &planeData_.verticesOffsets[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &planeData_.verticesOffsets[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &planeData_.verticesOffsets[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &planeData_.verticesOffsets[3].x, 0.01f);

	ImGui::End();

	// トランスフォーム更新
	worldTransform_.Update();
}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	// 板ポリ描画
	MAGISYSTEM::DrawPlane3D(worldTransform_.worldMatrix_, planeData_, material_);

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
