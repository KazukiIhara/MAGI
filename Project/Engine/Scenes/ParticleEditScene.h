#pragma once

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
	// カメラ
	std::unique_ptr<Camera3D> sceneCamera_ = nullptr;

};

template<typename Data>
inline void ParticleEditScene<Data>::Initialize() {
	// シーンカメラ作成
	sceneCamera_ = std::make_unique<Camera3D>("SceneCamera");
	MAGISYSTEM::AddCamera3D(std::move(sceneCamera_));

	// カメラの設定
	MAGISYSTEM::SetCurrentCamera("SceneCamera");

}

template<typename Data>
inline void ParticleEditScene<Data>::Update() {

}

template<typename Data>
inline void ParticleEditScene<Data>::Draw() {

}

template<typename Data>
inline void ParticleEditScene<Data>::Finalize() {

}
