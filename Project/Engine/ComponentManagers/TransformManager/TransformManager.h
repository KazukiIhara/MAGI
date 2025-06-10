#pragma once

// C++
#include <vector>
#include <memory>

// MyHedder
#include "Transform3D/Transform3D.h"

/// <summary>
/// トランスフォーム管理クラス
/// </summary>
class TransformManager {
public:
	TransformManager();
	~TransformManager();

	[[nodiscard]] std::weak_ptr<Transform3D> Add(std::shared_ptr<Transform3D> transform);
	void Update();
	void DeleteGarbage();
	void Clear();

private:
	// トランスフォームコンテナ
	std::vector<std::shared_ptr<Transform3D>> transforms_;
};