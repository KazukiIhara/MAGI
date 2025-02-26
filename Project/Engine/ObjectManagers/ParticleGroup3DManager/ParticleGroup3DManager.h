#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Enums/Primitive3DEnum.h"

#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"
#include "3D/ParticleGroups3D/PrimitiveParticleGroup3D/PrimitiveParticleGroup3D.h"

/// <summary>
/// パーティクルグループのマネージャ
/// </summary>
class ParticleGroup3DManager {
public:
	ParticleGroup3DManager();
	~ParticleGroup3DManager();

	void Update();
	void Draw();

	std::string CreatePrimitiveParticleGroup(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName);
	void Remove(const std::string& name);

	BaseParticleGroup3D* Find(const std::string& name);

private:
	// パーティクルグループコンテナ
	std::vector<std::unique_ptr<BaseParticleGroup3D>> particleGroups3D_;
};

