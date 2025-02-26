#pragma once

#include <string>
#include <map>

#include "Structs/EmitterStruct.h"

#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"
#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"

/// <summary>
/// 3Dパーティクルエミッター
/// </summary>
class Emitter3D :public WorldEntity {
public:
	Emitter3D(const std::string& emitterName, const Vector3& position);
	~Emitter3D();

	// 初期化
	void Initialize(const Vector3& position);

	// 更新
	void Update();

	// パーティクル発生
	void EmitAll();

	// 新規パーティクルグループ追加
	void AddParticleGroup(BaseParticleGroup3D* particleGroup);

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

	// エミッターの設定構造体
	EmitterSetting emitterSetting_{};

	// パーティクルグループのマップ
	std::map<std::string, BaseParticleGroup3D*> particleGroups_;

	// 頻度用時刻
	float frequencyTime_ = 0.0f;
};