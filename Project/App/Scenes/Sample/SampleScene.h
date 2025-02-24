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

	//
	// オブジェクトの作成
	//

	// ライト
	MAGISYSTEM::AddPunctualLight("sampleLight");

	// レンダラー
	MAGISYSTEM::CreateStaticRenderer3D("terrain", "terrain");

	// プレイヤーを作成
	player_ = std::make_unique<Player>("player");
	player_->AddRenderer3D(MAGISYSTEM::FindRenderer3D("terrain"));

	// マネージャにプレイヤーを追加
	MAGISYSTEM::CreateGameObject3D(std::move(player_));


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
