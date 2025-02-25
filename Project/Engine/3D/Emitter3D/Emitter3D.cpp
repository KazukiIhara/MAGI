#include "Emitter3D.h"

#include <cassert>

Emitter3D::Emitter3D() {

}

Emitter3D::~Emitter3D() {

}

void Emitter3D::Initialize() {

}

void Emitter3D::Update() {

}

void Emitter3D::EmitAll() {

}

void Emitter3D::AddParticleGroup(BaseParticleGroup3D* particleGroup) {
	assert(particleGroup);
	// 名前を取得してマップに入れる
	particleGroups_.insert(std::pair(particleGroup->name, particleGroup));
}
