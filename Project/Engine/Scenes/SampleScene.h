#pragma once

#include <memory>
#include <array>

#include "Framework/MAGI.h"

#include "2D/Object2D/Object2D.h"

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

	static const uint32_t primitiveSize_ = 65535;

	std::array<Vector3, 4> vertices_;

	// 板ポリ用のワールドトランスフォーム
	std::array<std::unique_ptr<WorldTransform>, primitiveSize_> primitiveWorldTransform_;
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
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));


	// カメラの設定
	MAGISYSTEM::SetCurrentCamera("SceneCamera");

	// ライト
	MAGISYSTEM::AddPunctualLight("SampleLight");

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

	for (size_t i = 0; i < primitiveSize_; i++) {
		primitiveWorldTransform_[i] = std::make_unique<WorldTransform>();
		primitiveWorldTransform_[i]->Initialize();
		primitiveWorldTransform_[i]->translate_.x = float(i);
	}
	material_.textureIndex = 1;
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	for (size_t i = 0; i < primitiveSize_; i++) {
		primitiveWorldTransform_[i]->Update();
	}

}

template<typename Data>
inline void SampleScene<Data>::Draw() {

	for (size_t i = 0; i < primitiveSize_; i++) {
		MAGISYSTEM::DrawTriangle3D(primitiveWorldTransform_[i]->worldMatrix_, material_);
	}

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
