#pragma once

#include <vector>
#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

// パーティクル作成シーン
template <typename Data>
class ParticleEditScene : public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~ParticleEditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// シーンUI描画
	void ShowSceneUI();

	// エミッターのエディットUI表示
	void ShowEmitterEditUI();

private:
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

	// エディットするエミッター
	Emitter3D* emitter3D_ = nullptr;
	// パーティクルリスト
	std::vector<BaseParticleGroup3D*> particles_;
};

template<typename Data>
inline void ParticleEditScene<Data>::Initialize() {

	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));

	// カメラの設定
	MAGISYSTEM::SetCurrentCamera("SceneCamera");


	// エディター用のエミッターを作成
	MAGISYSTEM::CreatePrimitiveParticleGroup3D("EditParticle", Primitive3DType::Plane);

	// エミッター作成
	MAGISYSTEM::CreateEmitter3D("EditEmitter", Vector3(0.0f, 0.0f, 0.0f));

	emitter3D_ = MAGISYSTEM::FindEmitter3D("EditEmitter");
	// エディター用のパーティクルを一つ作成
	emitter3D_->AddParticleGroup(MAGISYSTEM::FindParticleGroup3D("EditParticle"));
	// エミットタイプ変更
	emitter3D_->GetEmitterSetting().emitType = EmitType::Random;
	// リピートオン
	emitter3D_->GetEmitterSetting().isRepeat = true;
}

template<typename Data>
inline void ParticleEditScene<Data>::Update() {
	ShowSceneUI();
}

template<typename Data>
inline void ParticleEditScene<Data>::Draw() {

}

template<typename Data>
inline void ParticleEditScene<Data>::Finalize() {

}

template<typename Data>
inline void ParticleEditScene<Data>::ShowSceneUI() {
	ImGui::Begin("EmitterEditUI");

	ShowEmitterEditUI();

	ImGui::End();
}

template<typename Data>
inline void ParticleEditScene<Data>::ShowEmitterEditUI() {
	// 描画サンプルのエミッターをいじるUI
	ImGui::Text("EmitterParamater");
	// 発生パターン

	// 1度の発生個数
	int count = static_cast<int>(emitter3D_->GetEmitterSetting().count);
	ImGui::DragInt("EmitCount", &count);
	// 発生個数が0未満にならないようにする
	count = std::max(0, count);
	emitter3D_->GetEmitterSetting().count = static_cast<int>(count);
	// 発生頻度
	ImGui::DragFloat("Frequency", &emitter3D_->GetEmitterSetting().frequency, 0.01f);
	// 繰り返し発生するかどうか
	ImGui::Checkbox("IsRepeat", &emitter3D_->GetEmitterSetting().isRepeat);
	// 最小の発生地点
	ImGui::DragFloat3("MinTranslate", &emitter3D_->GetEmitterSetting().minTranslate.x, 0.01f);
	// 最大の発生地点
	ImGui::DragFloat3("MaxTranslate", &emitter3D_->GetEmitterSetting().maxTranslate.x, 0.01f);
	// 最小移動量
	ImGui::DragFloat3("MinVelocity", &emitter3D_->GetEmitterSetting().minVelocity.x, 0.01f);
	// 最大移動量
	ImGui::DragFloat3("MaxVelocity", &emitter3D_->GetEmitterSetting().maxVelocity.x, 0.01f);
	// 最小サイズ
	ImGui::DragFloat("MinScale", &emitter3D_->GetEmitterSetting().minScale, 0.01f);
	// 最大サイズ
	ImGui::DragFloat("MaxScale", &emitter3D_->GetEmitterSetting().maxScale, 0.01f);
	// 最短生存時間
	ImGui::DragFloat("MinLifeTime", &emitter3D_->GetEmitterSetting().minLifeTime, 0.01f);
	// 最長生存時間
	ImGui::DragFloat("MaxLifeTime", &emitter3D_->GetEmitterSetting().maxLifeTime, 0.01f);
	// 最小の色
	ImGui::ColorEdit4("MinColor", &emitter3D_->GetEmitterSetting().minColor.r);
	// 最大の色
	ImGui::ColorEdit4("MaxColor", &emitter3D_->GetEmitterSetting().maxColor.r);

	// パーティクル追加ボタン
	if (ImGui::Button("AddParticle")) {

	}

}