#include "BaseParticleGroup3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseParticleGroup3D::BaseParticleGroup3D() {}

BaseParticleGroup3D::~BaseParticleGroup3D() {}

void BaseParticleGroup3D::Update() {
	// 描画すべきインスタンス数
	instanceCount_ = 0;

	for (std::list<ParticleData>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {
		// 生存時間を過ぎていたら更新せず描画対象にしない
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 経過時間を足す
		(*particleIterator).currentTime += MAGISYSTEM::GetDeltaTime();
		// 移動
		(*particleIterator).transform.translate += MAGISYSTEM::GetDeltaTime() * (*particleIterator).velocity;
		// 透明度
		float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

		if (instanceCount_ < kNumMaxInstance_) {
			// Wマトリックスを求める
			// Scale
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
			// 
			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix((*particleIterator).transform.rotate);

			// translate
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);

			// ワールド行列を作成
			Matrix4x4 worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

			// ワールド行列
			instancingData_[instanceCount_].World = worldMatrix;
			// 色を入力
			instancingData_[instanceCount_].color.x = (*particleIterator).color.x;
			instancingData_[instanceCount_].color.y = (*particleIterator).color.y;
			instancingData_[instanceCount_].color.z = (*particleIterator).color.z;
			instancingData_[instanceCount_].color.w = alpha;

			// 生きているParticleの数を1つカウントする
			instanceCount_++;
		}
		// 次のイテレーターに進める
		++particleIterator;
	}
}

void BaseParticleGroup3D::AddNewParticle(const EmitParamater& emitSetting) {
	// 最大数を超えていたら追加しない
	if (particles_.size() >= kNumMaxInstance_) {
		return;
	}

	// 追加するパーティクル
	ParticleData particle;
	// トランスフォームの設定
	particle.transform.scale = emitSetting.scale;
	particle.transform.rotate = emitSetting.rotate;
	particle.transform.translate = emitSetting.position;
	// 移動量の設定
	particle.velocity = emitSetting.velocity;
	// 色の設定
	particle.color = emitSetting.color;

	// 生存時間の設定
	particle.lifeTime = emitSetting.lifeTime;
	particle.currentTime = 0;

	// コンテナに挿入
	particles_.push_back(particle);
}

void BaseParticleGroup3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = MAGISYSTEM::CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance_);
}

void BaseParticleGroup3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumMaxInstance_; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void BaseParticleGroup3D::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Material3D));
}

void BaseParticleGroup3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
}
