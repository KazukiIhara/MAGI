#pragma once

#include <map>
#include <string>

#include "3D/ParticleGroups3D/BaseParticleGroup3D/BaseParticleGroup3D.h"

/// <summary>
/// パーティクルグループのマネージャ
/// </summary>
class ParticleGroup3DManager {
public:
	ParticleGroup3DManager();
	~ParticleGroup3DManager();

	void Update();
	void Draw();

	std::string CreatePrimitiveParticleGroup();

private:


};

