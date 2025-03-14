#include "Emitter3D.h"

#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"
#include "Random/Random.h"

#include <cassert>

using namespace MAGIMath;
using namespace MAGIUtility;

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
		frequencyTimer_ += MAGISYSTEM::GetDeltaTime();

		// 頻度より大きいなら発生
		if (emitterSetting_.frequency <= frequencyTimer_) {
			EmitAll();
			// 余計に過ぎた時間も加味して頻度計算する
			frequencyTimer_ -= emitterSetting_.frequency;
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
		// パーティクルグループの個数分ループ
		for (auto particleGroup : particleGroups_) {
			// 発生個数分ループ
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

				// 色
				emitParamater.color.x = Random::GenerateFloat(emitterSetting_.minColor.r, emitterSetting_.maxColor.r);
				emitParamater.color.y = Random::GenerateFloat(emitterSetting_.minColor.g, emitterSetting_.maxColor.g);
				emitParamater.color.z = Random::GenerateFloat(emitterSetting_.minColor.b, emitterSetting_.maxColor.b);
				emitParamater.color.w = Random::GenerateFloat(emitterSetting_.minColor.a, emitterSetting_.maxColor.a);

				// 生存時間
				emitParamater.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

				particleGroup.second->AddNewParticle(emitParamater);
			}
		}
		break;
	case EmitType::Random:
		// パーティクルグループの個数分ループ
		for (auto particleGroup : particleGroups_) {
			// 発生個数分ループ
			for (uint32_t i = 0; i < emitterSetting_.count; i++) {
				// 発生座標
				emitParamater.position.x = worldPosition.x + Random::GenerateFloat(emitterSetting_.minTranslate.x, emitterSetting_.maxTranslate.x);
				emitParamater.position.y = worldPosition.y + Random::GenerateFloat(emitterSetting_.minTranslate.y, emitterSetting_.maxTranslate.y);
				emitParamater.position.z = worldPosition.z + Random::GenerateFloat(emitterSetting_.minTranslate.z, emitterSetting_.maxTranslate.z);
				// サイズ
				float size = Random::GenerateFloat(emitterSetting_.minScale, emitterSetting_.maxScale);
				emitParamater.scale.x = size;
				emitParamater.scale.y = size;
				emitParamater.scale.z = size;
				// 移動量
				emitParamater.velocity.x = Random::GenerateFloat(emitterSetting_.minVelocity.x, emitterSetting_.maxVelocity.x);
				emitParamater.velocity.y = Random::GenerateFloat(emitterSetting_.minVelocity.y, emitterSetting_.maxVelocity.y);
				emitParamater.velocity.z = Random::GenerateFloat(emitterSetting_.minVelocity.z, emitterSetting_.maxVelocity.z);
				// 色
				emitParamater.color.x = Random::GenerateFloat(emitterSetting_.minColor.r, emitterSetting_.maxColor.r);
				emitParamater.color.y = Random::GenerateFloat(emitterSetting_.minColor.g, emitterSetting_.maxColor.g);
				emitParamater.color.z = Random::GenerateFloat(emitterSetting_.minColor.b, emitterSetting_.maxColor.b);

				// 生存時間
				emitParamater.lifeTime = Random::GenerateFloat(emitterSetting_.minLifeTime, emitterSetting_.maxLifeTime);

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

EmitterSetting& Emitter3D::GetEmitterSetting() {
	return emitterSetting_;
}
