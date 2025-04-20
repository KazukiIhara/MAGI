#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>

// MyHedder
#include "Structs/ModelStruct.h"
#include "Math/Utility/MathUtility.h"
#include "3D/Drawer3D/ModelDrawer3D/ModelDrawer3D.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

/// <summary>
/// モデル描画クラスのマネージャー
/// </summary>
class ModelDrawer3DManager {
public:
	ModelDrawer3DManager(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~ModelDrawer3DManager();

	void CreateModelDrawer(const std::string& modelName, const ModelData& modelData);
	void DrawModel(const std::string& modelName, const Matrix4x4& worldMatrix);
	void UpdateAll();
	void DrawAll();

private:
	// 描画クラスのコンテナ
	std::unordered_map<std::string, std::unique_ptr<ModelDrawer3D>> modelDrawers_;
};