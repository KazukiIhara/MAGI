#pragma once

#include <memory>

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

	// ゲームオブジェクト作成
	std::unique_ptr<GameObject3D> terrainObject = std::make_unique<GameObject3D>("terrain");

	std::unique_ptr<GameObject3D> terrainObject2 = std::make_unique<GameObject3D>("terrain2");
	terrainObject2->GetTranslate().y = 1.0f;

	std::unique_ptr<GameObject3D> terrainObject3 = std::make_unique<GameObject3D>("terrain3");
	terrainObject3->GetTranslate().y = -1.0f;


	// ゲームオブジェクトグループ作成
	std::unique_ptr<GameObject3DGroup> terrainGroup = std::make_unique<GameObject3DGroup>("TerrainGroup");
	// グループにレンダラーを追加
	terrainGroup->AddRenderer(std::move(terrain));

	// グループにゲームオブジェクトを追加
	terrainGroup->AddObject(std::move(terrainObject));
	terrainGroup->AddObject(std::move(terrainObject2));
	terrainGroup->AddObject(std::move(terrainObject3));

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

	// 音声再生
	MAGISYSTEM::PlayLoopWaveSound("Alarm01.wav");

}

template<typename Data>
inline void SampleScene<Data>::Update() {

}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
