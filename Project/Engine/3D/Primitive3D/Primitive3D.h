#pragma once

// C++
#include <memory>

#include "3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/PrimitiveShapes3D/BasePrimitiveShape3D/BasePrimitiveShape3D.h"
#include "Enums/Primitive3DEnum.h"

class Primitive3D:public BaseRenderable3D {
public:
	Primitive3D(const std::string& objectName, const Primitive3DType& primitiveType, const std::string& textureFilePath = "");
	~Primitive3D()override;
	void Initialize(const Primitive3DType& primitiveType, const std::string& textureFilePath);
	void Update()override;
	void Draw()override;
private:
	// 形状の設定
	Primitive3DType primitiveType_{};
	// 形状
	std::unique_ptr<BasePrimitiveShape3D> primitive_ = nullptr;
};