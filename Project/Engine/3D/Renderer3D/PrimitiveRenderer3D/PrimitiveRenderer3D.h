#pragma once

// MyHedder
#include "Framework/MAGI.h"

/// <summary>
/// 3Dプリミティブの種類
/// </summary>
enum class Prim3DType {
	Triangle,
	Plane,
	Box,
	Sphere,
};

/// <summary>
/// 3Dプリミティブ描画
/// </summary>
template<typename T>
class PrimitiveRenderer3D {
public:
	PrimitiveRenderer3D(const std::string& name, const Prim3DType& type) {
		// 名前を設定
		name_ = name;
		// タイプをセット
		type_ = type;
		// トランスフォームコンポーネントを作成
		std::unique_ptr<Transform3D> transform = std::make_unique<Transform3D>();
		transform_ = MAGISYSTEM::AddTransform3D(std::move(transform));
	}

	~PrimitiveRenderer3D() = default;

	void Draw() {

	}

	void SetShapeData(const T& data) {

	}

	Transform3D* GetTransform() {
		return transform_; 
	}

private:
	// 名前
	std::string name_ = "";
	// プリミティブタイプ
	Prim3DType type_ = Prim3DType::Triangle;
	// トランスフォーム
	Transform3D* transform_ = nullptr;
	// 形状データ
	T shapeData_;
	// 描画フラグ
	bool isRender_ = true;
};