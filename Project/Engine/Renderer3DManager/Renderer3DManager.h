#pragma once

#include <vector>
#include <string>

#include "Enums/Renderer3DEnum.h"
#include "Enums/Primitive3DEnum.h"
#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/RenderObjects3D/PrimitiveRenderer3D/PrimitiveRenderer3D.h"
#include "3D/RenderObjects3D/StaticRenderer3D/StaticRenderer3D.h"
#include "3D/RenderObjects3D/SkinningRenderer3D/SkinningRenderer3D.h"

/// <summary>
/// 3D描画オブジェクトマネージャ
/// </summary>
class Renderer3DManager {
public:
	Renderer3DManager();
	~Renderer3DManager();

	void Update();
	void Draw();

	std::string CreatePrimitiveRenderer(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName);
	std::string CreateStaticRenderer(const std::string& name, const std::string& modelName);
	std::string CreateSkinningRenderer(const std::string& name, const std::string& modelName);
	void Remove(const std::string& name);

	BaseRenderable3D* Find(const std::string& name);

	void Clear();

	// 生成してある全オブジェクトのリストを取得
	const std::vector<std::unique_ptr<BaseRenderable3D>>& GetRenderers() const;

private:
	// 描画オブジェクトコンテナ
	std::vector<std::unique_ptr<BaseRenderable3D>> renderers_;

};