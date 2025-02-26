#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "2D/Object2D/Object2D.h"

#include "GameObjects/Player/Player.h"

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
	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	Emitter3D* emitter_ = nullptr;
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

	//
	// オブジェクトの作成
	//

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
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("Plane", Primitive3DType::Sphere);

	// エミッターを作成
	MAGISYSTEM::CreateEmitter3D("Emitter", Vector3(0.0f, 0.0f, 0.0f));
	emitter_ = MAGISYSTEM::FindEmitter3D("Emitter");

	// エミッターにパーティクルを挿入
	emitter_->AddParticleGroup(MAGISYSTEM::FindParticleGroup3D("Plane"));
	emitter_->GetEmitterSetting().isRepeat = true;
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
