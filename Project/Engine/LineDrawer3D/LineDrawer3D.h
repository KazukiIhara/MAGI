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

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

class LineDrawer3D {
public:
	LineDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager);
	~LineDrawer3D();

	void Update();
	void Draw();

	void AddLine(const Vector3& start, const Vector3& end, const RGBA& color);
	void ClearLines();

private:
	void Initialize(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager);
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
	// ラインの最大数
	const uint32_t kNumMaxInstance = 16384;

	// ライン
	std::vector<LineData3D> lines_;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	LineData3D* instancingData_ = nullptr;

	// SrvIndex
	uint32_t srvIndex_ = 0;
	// instance描画する際に使う変数
	uint32_t instanceCount_ = kNumMaxInstance;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;

};