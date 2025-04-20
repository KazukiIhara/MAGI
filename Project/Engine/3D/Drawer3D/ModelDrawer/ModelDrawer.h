#pragma once

// MyHedder
#include "Math/Utility/MathUtility.h"
#include "Structs/ModelStruct.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

/// <summary>
/// モデル描画用クラス
/// </summary>
class ModelDrawer {
public:
	ModelDrawer(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager,
		const ModelData& modelData
	);
	~ModelDrawer();

	void AddDrawCommand(const Matrix4x4& worldMatrix);
	void Update();
	void Draw();

private:
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);

private:


private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
};