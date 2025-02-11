#include "Collider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

Collider3D::Collider3D(GameObject3D* owner) {
	owner_ = owner;
}

Collider3D::~Collider3D() {}
