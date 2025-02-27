#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

// パーティクル作成シーン
template <typename Data>
class ParticleEditScene: public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~ParticleEditScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

};

template<typename Data>
inline void ParticleEditScene<Data>::Initialize() {

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
