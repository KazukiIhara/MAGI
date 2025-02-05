#pragma once

// C++
#include <memory>

#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/Shapes3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"
#include "Enums/Primitive3DEnum.h"

/// <summary>
/// シンプル形状描画クラス
/// </summary>
class Primitive3D:public BaseRenderable3D {
public:
	Primitive3D(const std::string& objectName, const Primitive3DType& primitiveType, const std::string& textureName = "");
	~Primitive3D()override;
	void AssignShape()override;
	void Update()override;
	void Draw()override;
private:
	// 形状の設定
	Primitive3DType primitiveType_{};
	// 貼り付けるテクスチャファイル
	std::string textureName_ = "";
	// 法線マップ用のテクスチャファイル
	std::string normalMapTextureName_ = "";
	// 形状
	std::unique_ptr<BasePrimitiveShape3D> primitive_ = nullptr;
};