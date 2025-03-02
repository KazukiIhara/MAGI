#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "2D/Object2D/Object2D.h"

#include "GameObjects/Player/Player.h"

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
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
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
	MAGISYSTEM::AddPunctualLight("sampleLight");

	// レンダラー
	MAGISYSTEM::CreateStaticRenderer3D("terrain", "terrain");

	// ゲームオブジェクト

	// プレイヤーを作成
	player_ = std::make_unique<Player>("player");
	player_->AddRenderer3D(MAGISYSTEM::FindRenderer3D("terrain"));

	// マネージャにプレイヤーを追加
	MAGISYSTEM::AddGameObject3D(std::move(player_));

	// パーティクルを作成
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Plane", Primitive3DType::Plane);

	// エミッターを作成
	MAGISYSTEM::CreateEmitter3D("Emitter", Vector3(0.0f, 0.0f, 0.0f));

	// エミッターにパーティクルを挿入
	MAGISYSTEM::FindEmitter3D("Emitter")->AddParticleGroup(MAGISYSTEM::FindParticleGroup3D("Plane"));

	// エミッターの設定
	MAGISYSTEM::FindEmitter3D("Emitter")->GetEmitterSetting().isRepeat = true;

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
