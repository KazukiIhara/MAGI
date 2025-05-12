#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera2DManager;

/// <summary>
/// スプライト描画クラス
/// </summary>
class SpriteDrawer {
public:
	SpriteDrawer(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera2DManager* camera2DManager
	);
	~SpriteDrawer();

	void Update();
	void Draw(BlendMode blendMode);

	void AddSprite(
		const Vector2& screenPosition
	);

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera2DManager(Camera2DManager* camera2DManager);

private:
	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_[static_cast<uint32_t>(BlendMode::Num)];

	// マテリアルのリソース
	ComPtr<ID3D12Resource> materialResource_[static_cast<uint32_t>(BlendMode::Num)];


	// SpriteSrvIndex
	uint32_t instancingSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];
	// MaterialSrvIndex
	uint32_t materialSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];

	// instance描画する際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];

	// 現在のインデックス
	uint32_t currentIndex_[static_cast<uint32_t>(BlendMode::Num)];
private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera2DManager* camera2DManager_ = nullptr;
};