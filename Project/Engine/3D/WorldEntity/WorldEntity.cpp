#include "WorldEntity.h"

using namespace MAGIMath;

WorldEntity::WorldEntity() {
	name_ = "";
	worldPosition_ = MakeZeroVector3();
}

std::string& WorldEntity::GetName() {
	return name_;
}
