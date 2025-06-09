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

void GameObject3D::Finalize() {
	transformComponent_->SetIsAlive(false);
}

void GameObject3D::AddModelRenderer(std::unique_ptr<ModelRenderer> modelRenderer) {
	ModelRenderer* ptr = MAGISYSTEM::AddRenderer3D(std::move(modelRenderer));
	modelRendererComponents_.insert(std::make_pair(ptr->GetName(), ptr));
}

void GameObject3D::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void GameObject3D::SetIsActive(bool isActive) {
	isActive_ = isActive;
}

const bool& GameObject3D::GetIsAlive()const {
	return isAlive_;
}

const bool& GameObject3D::GetIsActive() const {
	return isActive_;
}

Transform3D* GameObject3D::GetTransform() {
	return transformComponent_;
}
