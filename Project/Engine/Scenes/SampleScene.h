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

	static const uint32_t planeSize_ = 1;

	std::array<Vector3, 4> vertices_;

	// 板ポリ用のワールドトランスフォーム
	std::array<std::unique_ptr<WorldTransform>, planeSize_> planeWorldTransform_;
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
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));


	// カメラの設定
	MAGISYSTEM::SetCurrentCamera("SceneCamera");

	// ライト
	MAGISYSTEM::AddPunctualLight("SampleLight");


	// レンダラー作成
	std::unique_ptr<StaticRenderer3D> terrain = MAGISYSTEM::CreateStaticRenderer3D("terrain", "terrain");


	// ゲームオブジェクトグループ作成
	std::unique_ptr<GameObject3DGroup> terrainGroup = std::make_unique<GameObject3DGroup>("TerrainGroup");
	// グループにレンダラーを追加
	terrainGroup->AddRenderer(std::move(terrain));

	// ゲームオブジェクト作成
	std::array<std::unique_ptr<GameObject3D>, 1500> terrainObject;

	//for (uint32_t i = 0; i < 1500; i++) {
	//	std::string number = std::to_string(i);

	//	terrainObject[i] = std::make_unique<GameObject3D>("terrain" + number);
	//	terrainObject[i]->GetTranslate().x = static_cast<float>(i);
	//	// グループにゲームオブジェクトを追加
	//	terrainGroup->AddObject(std::move(terrainObject[i]));
	//}

	// ゲームオブジェクトグループを追加
	MAGISYSTEM::AddGameObejct3DGroup(std::move(terrainGroup));

	// パーティクルを作成
	MAGISYSTEM::CreateStaticParticleGroup3D("Plane", "terrain");

	// エミッターを作成
	MAGISYSTEM::CreateEmitter3D("Emitter", Vector3(0.0f, 0.0f, 0.0f));

	// エミッターをマネージャから取得
	Emitter3D* emitter = MAGISYSTEM::FindEmitter3D("Emitter");

	// エミッターにパーティクルを挿入
	emitter->AddParticleGroup(MAGISYSTEM::FindParticleGroup3D("Plane"));

	// エミッターの設定
	// ランダム拡散
	emitter->GetEmitterSetting().emitType = EmitType::Random;
	// 発生個数を5に
	emitter->GetEmitterSetting().count = 5;
	// 移動量を-1～1に
	emitter->GetEmitterSetting().maxVelocity = { 1.0f,1.0f,1.0f };
	emitter->GetEmitterSetting().minVelocity = { -1.0f,-1.0f,-1.0f };

	for (uint32_t i = 0; i < planeSize_; i++) {
		planeWorldTransform_[i] = std::make_unique<WorldTransform>();
		planeWorldTransform_[i]->Initialize();
		planeWorldTransform_[i]->translate_.x = static_cast<float>(i) * 2.0f;
	}

	// 板ポリの
	vertices_[0] = Vector3(-1.0f, 1.0f, 0.0f);
	vertices_[1] = Vector3(1.0f, 1.0f, 0.0f);
	vertices_[2] = Vector3(-1.0f, -1.0f, 0.0f);
	vertices_[3] = Vector3(1.0f, -1.0f, 0.0f);
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	for (uint32_t i = 0; i < planeSize_; i++) {
		planeWorldTransform_[i]->Update();
	}

	ImGui::Begin("PlaneVertices");

	ImGui::DragFloat3("LeftTop", &vertices_[0].x, 0.01f);
	ImGui::DragFloat3("RightTop", &vertices_[1].x, 0.01f);
	ImGui::DragFloat3("LeftBottom", &vertices_[2].x, 0.01f);
	ImGui::DragFloat3("RightBottom", &vertices_[3].x, 0.01f);

	ImGui::End();

}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	for (uint32_t i = 0; i < planeSize_; i++) {
		// 板ポリ描画
		MAGISYSTEM::DrawPlane3D(
			planeWorldTransform_[i]->worldMatrix_,
			vertices_[0],
			vertices_[1],
			vertices_[2],
			vertices_[3],
			Color::White,
			1,
			Vector2(1.0f,1.0f),
			0.0f,
			Vector2(0.0f,0.0f));
	}

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
