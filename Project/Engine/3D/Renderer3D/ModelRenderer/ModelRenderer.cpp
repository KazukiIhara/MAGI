#include "ModelRenderer.h"

// MyHedder
#include "Framework/MAGI.h"

ModelRenderer::ModelRenderer(const std::string& name, const std::string& modelName, const ModelMaterial& material) {
	name_ = name;
	modelName_ = modelName;
	material_ = material;

	// トランスフォームを作成
	std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
	transform_ = MAGISYSTEM::AddTransform3D(std::move(transform));

}

ModelRenderer::~ModelRenderer() {

}

void ModelRenderer::Draw() {
	if (isRender_) {
		MAGISYSTEM::DrawModel(modelName_, transform_->GetWorldMatrix(), material_);
	}
}

void ModelRenderer::SetMaterial(const ModelMaterial& material) {
	material_ = material;
}

void ModelRenderer::SetIsRender(bool isRender) {
	isRender_ = isRender;
}
