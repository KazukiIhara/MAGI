#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"

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

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	ModelDataForGPU* instancingData_ = nullptr;

	// instance描画の際に使う変数
	uint32_t instanceCount_ = 0;
	// 現在のインデックス
	uint32_t currentIndex_ = 0;
};