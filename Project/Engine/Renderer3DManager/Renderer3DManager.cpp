#include "Renderer3DManager.h"

#include "Logger/Logger.h"

#include <cassert>

Renderer3DManager::Renderer3DManager() {
	Clear();
	Logger::Log("Renderer3DManager Initialize\n");
}

Renderer3DManager::~Renderer3DManager() {
	Logger::Log("Renderer3DManager Finalize\n");
}

void Renderer3DManager::Update() {
	for (auto& renderer : renderers_) {
		if (renderer) {
			renderer->Update();
		}
	}
}

void Renderer3DManager::Draw() {
	for (auto& renderer : renderers_) {
		if (renderer) {
			renderer->Draw();
		}
	}
}

void Renderer3DManager::CreatePrimitiveRenderer(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName) {
	// 追加する描画オブジェクト
	std::unique_ptr<BaseRenderable3D> newRenderer3D = std::make_unique<PrimitiveRenderer3D>(name, primitiveRenderer, textureName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Primitive);
	// コンテナに登録
	renderers_.push_back(std::move(newRenderer3D));
}

void Renderer3DManager::CreateStaticRenderer(const std::string& name, const std::string& modelName) {
	// 追加する描画オブジェクト
	std::unique_ptr<BaseRenderable3D> newRenderer3D = std::make_unique<StaticRenderer3D>(name, modelName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Static);
	// コンテナに登録
	renderers_.push_back(std::move(newRenderer3D));
}

void Renderer3DManager::CreateSkinningRenderer(const std::string& name, const std::string& modelName) {
	// 追加する描画オブジェクト
	std::unique_ptr<BaseRenderable3D> newRenderer3D = std::make_unique<SkinningRenderer3D>(name, modelName);
	// 形状を設定
	newRenderer3D->AssignShape();
	// 形状タイプを設定
	newRenderer3D->SetRenderer3DType(Renderer3DType::Skinning);
	// コンテナに登録
	renderers_.push_back(std::move(newRenderer3D));
}

void Renderer3DManager::Remove(const std::string& name) {
	// ベクターを走査して、名前が一致する描画オブジェクトを探す
	for (auto it = renderers_.begin(); it != renderers_.end(); ++it) {
		// (*it)->name_ で描画オブジェクト名名を取得
		if ((*it)->name_ == name) {
			renderers_.erase(it);
			return;  // 見つかったら削除して関数を抜ける
		}
	}
	// 見つからなかった場合
	assert(false && "Not Found Renderer3D to Remove");
}

BaseRenderable3D* Renderer3DManager::Find(const std::string& name) {
	// ベクターを走査して、名前が一致する描画オブジェクトを探す
	for (auto& renderer : renderers_) {
		if (renderer && renderer->name_ == name) {
			return renderer.get();  // ポインタを返す
		}
	}

	// 見つからない場合
	assert(false && "Not Found Renderer3D");
	return nullptr;
}

void Renderer3DManager::Clear() {
	renderers_.clear();
}

const std::vector<std::unique_ptr<BaseRenderable3D>>& Renderer3DManager::GetRenderers() const {
	return renderers_;
}
