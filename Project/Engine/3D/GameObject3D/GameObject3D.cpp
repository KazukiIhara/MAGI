#include "GameObject3D.h"

#include <memory>

#include "framework/MAGI.h"
#include "MAGIAssert/MAGIAssert.h"

GameObject3D::GameObject3D(const std::string& name) {
	name_ = name;
	std::shared_ptr<Transform3D> transform = std::make_shared<Transform3D>();
	transformComponent_ = MAGISYSTEM::AddTransform3D(std::move(transform));
}

GameObject3D::~GameObject3D() {

}

void GameObject3D::Update() {

}

void GameObject3D::Finalize() {
	{
		if (auto it = transformComponent_.lock()) {
			it->SetIsAlive(false);
		}
	}

	{
		if (!modelRendererComponents_.empty()) {
			for (auto& modelRenderer : modelRendererComponents_) {
				if (auto it = modelRenderer.second.lock()) {
					it->SetIsAlive(false);
				}
			}
		}
	}
}

void GameObject3D::AddModelRenderer(std::shared_ptr<ModelRenderer> modelRenderer) {
	std::weak_ptr<ModelRenderer> ptr = MAGISYSTEM::AddRenderer3D(std::move(modelRenderer));
	if (auto p = ptr.lock()) {
		modelRendererComponents_.insert(std::make_pair(p->GetName(), p));
	}
}

void GameObject3D::SetIsAlive(bool isAlive) {
	isAlive_ = isAlive;
}

void GameObject3D::SetIsActive(bool isActive) {
	isActive_ = isActive;
}

bool GameObject3D::GetIsAlive()const {
	return isAlive_;
}

bool GameObject3D::GetIsActive() const {
	return isActive_;
}

std::weak_ptr<Transform3D> GameObject3D::GetTransform() {
	return transformComponent_;
}

std::weak_ptr<ModelRenderer> GameObject3D::GetModelRenderer(const std::string& rendererName) {
	const auto it = modelRendererComponents_.find(rendererName);
	if (it != modelRendererComponents_.end()) {
		return it->second;
	}

	MAGIAssert::Assert(false, "GameObject3D " + name_ + ": Not found renderer [ " + rendererName + " ]\n");

	return {};
}
