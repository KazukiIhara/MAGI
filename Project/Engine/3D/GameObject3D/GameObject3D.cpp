#include "GameObject3D.h"

#include <memory>

#include "framework/MAGI.h"

GameObject3D::GameObject3D(const std::string& name) {
	name_ = name;
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
	transformComponent_ = MAGISYSTEM::AddTransform3D(std::move(transform));
}

GameObject3D::~GameObject3D() {

}

void GameObject3D::Update() {

}

Transform3D* GameObject3D::GetTransform() {
	return transformComponent_;
}
