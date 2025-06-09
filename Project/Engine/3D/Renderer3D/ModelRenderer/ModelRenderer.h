#pragma once

// C++
#include <string>

// MyHedder
#include "Structs/ModelStruct.h"

// 前方宣言
class Transform3D;

/// <summary>
/// モデル描画オブジェクト
/// </summary>
class ModelRenderer {
public:
	ModelRenderer(const std::string& name, const std::string& modelName, const ModelMaterial& material);
	~ModelRenderer();

	void Draw();

	void Finalize();

	void SetIsAlive(bool isAlive);

	void SetMaterial(const ModelMaterial& material);
	void SetIsRender(bool isRender);

	[[nodiscard]] const bool& GetisAlive()const;

private:
	// 名前
	std::string name_ = "";
	// モデルの名前
	std::string modelName_ = "";
	// マテリアル
	ModelMaterial material_{};
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// 描画フラグ
	bool isRender_ = true;
	// 生存フラグ
	bool isAlive_ = true;
};