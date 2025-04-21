#pragma once

// C++
#include <memory>

// MyHedder
#include "3D/Drawer3D/MeshDrawer/MeshDrawer.h"

/// <summary>
/// モデル描画用クラス
/// </summary>
class ModelDrawer {
public:
	ModelDrawer(const ModelData& modelData);
	~ModelDrawer();

	void AddDrawCommand(const Matrix4x4& worldMatrix);
	void Update();
	void Draw();

private:
	// メッシュ
	std::vector<std::unique_ptr<MeshDrawer>> meshes_;

};