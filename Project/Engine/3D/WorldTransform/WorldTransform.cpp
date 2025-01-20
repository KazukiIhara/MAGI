#include "WorldTransform.h"

using namespace MAGIMath;

void WorldTransform::Initialize() {
	WorldEntity::Initialize();
	scale_ = { 1.0f,1.0f,1.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	translate_ = { 0.0f,0.0f,0.0f };
	worldMatrix_ = MakeIdentityMatrix4x4();
}

void WorldTransform::Update() {

}
