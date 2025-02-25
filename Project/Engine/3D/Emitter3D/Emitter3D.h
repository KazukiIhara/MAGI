#pragma once

#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"

/// <summary>
/// 3Dパーティクルエミッター
/// </summary>
class Emitter3D :public WorldEntity {
public:
	Emitter3D();
	~Emitter3D();

	// 初期化
	void Initialize();

	// 更新
	void Update();

private:
	// パーティクルリスト

};