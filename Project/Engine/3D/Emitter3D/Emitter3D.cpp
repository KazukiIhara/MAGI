#include "Emitter3D.h"

#include "Framework/MAGI.h"
#include "Random/Random.h"

#include <cassert>

using namespace MAGIMath;

Emitter3D::Emitter3D(const std::string& emitterName, const Vector3& position) {
	name = emitterName;
	Initialize(position);
}

Emitter3D::~Emitter3D() {

}

void Emitter3D::Initialize(const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	worldTransform_.Update();
}

void Emitter3D::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.Update();
	// ワールド座標の更新
	worldPosition = ExtractionWorldPos(worldTransform_.worldMatrix_);

	// 繰り返しフラグがオンなら繰り返し発生
	if (emitterSetting_.isRepeat) {

		// エミッターの処理
		// 時刻を進める
		frequencyTime_ += MAGISYSTEM::GetDeltaTime();

		// 頻度より大きいなら発生
		if (emitterSetting_.frequency <= frequencyTime_) {
			EmitAll();
			// 余計に過ぎた時間も加味して頻度計算する
			frequencyTime_ -= emitterSetting_.frequency;
		}
	}
}

void Emitter3D::EmitAll() {
	// パーティクル発生パラメータ
	EmitParamater emitParamater{};
	// パーティクル発生ポジション
	emitParamater.position = worldPosition;
	// 発生タイプごとの処理
	switch (emitterSetting_.emitType) {
		// デフォルト
	case EmitType::Default:
		// 発生個数分ループ
		for (auto particleGroup : particleGroups_) {
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {

				// サイズ
				float scale = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater.scale.x = scale;
				emitParamater.scale.y = scale;
				emitParamater.scale.z = scale;

				// 発生座標
				emitParamater.position.x = worldPosition.x + Random::GenerateFloat(emitterSetting_.minTranslate.x, emitterSetting_.maxTranslate.x);
				emitParamater.position.y = worldPosition.y + Random::GenerateFloat(emitterSetting_.minTranslate.y, emitterSetting_.maxTranslate.y);
				emitParamater.position.z = worldPosition.z + Random::GenerateFloat(emitterSetting_.minTranslate.z, emitterSetting_.maxTranslate.z);

				particleGroup.second->AddNewParticle(emitParamater);
			}
		}
		break;
	}

}

void Emitter3D::AddParticleGroup(BaseParticleGroup3D* particleGroup) {
	assert(particleGroup);
	// 名前を取得してマップに入れる
	particleGroups_.insert(std::pair(particleGroup->name, particleGroup));
}
