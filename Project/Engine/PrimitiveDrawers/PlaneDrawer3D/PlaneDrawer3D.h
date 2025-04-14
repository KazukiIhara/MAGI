#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ObjectStruct.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

/// <summary>
/// 板ポリ描画クラス
/// </summary>
class PlaneDrawer3D {
public:
	PlaneDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~PlaneDrawer3D();

	void Update();
	void Draw();

	void AddPlane(const WorldTransform& worldTransform,
		float leftTop,
		float rightTop,
		float leftBottom,
		float rightBottom,
		const RGBA& color
	);

	void ClearPlanes();
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);
private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingデータ書き込み
	void MapInstancingData();
private:
	// 板ポリの最大数
	const uint32_t kNumMaxInstance = 32768;

	// 板ポリ
	std::vector<PlaneData3D> planes_;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	PlaneData3D* instancingData_ = nullptr;	

	// SrvIndex
	uint32_t srvIndex_ = 0;
	// instance描画する際に使う変数
	uint32_t instanceCount_ = 0;

	// 現在のインデックス
	uint32_t currentIndex_ = 0;
private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
};