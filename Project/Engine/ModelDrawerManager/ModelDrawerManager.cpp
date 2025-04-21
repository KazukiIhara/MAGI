#include "ModelDrawerManager.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

ModelDrawerManager::ModelDrawerManager(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera3DManager* camera3DManager
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);

	Logger::Log("ModelDrawerManager Initialize\n");
}

ModelDrawerManager::~ModelDrawerManager() {
	Logger::Log("ModelDrawerManager Initialize\n");
}

void ModelDrawerManager::CreateModelDrawer(const std::string& modelDrawerName, const ModelData& modelData) {
	// 追加する描画クラスを作成
	std::unique_ptr<ModelDrawer> newModelDrawer = std::make_unique<ModelDrawer>(modelData);
		
	// ペアを作って挿入
	modelDrawers_.insert(std::make_pair(modelDrawerName, std::move(newModelDrawer)));
}

void ModelDrawerManager::DrawModel(const std::string& modelDrawerName, const Matrix4x4& worldMatrix) {
	auto it = modelDrawers_.find(modelDrawerName);
	if (it != modelDrawers_.end()) {
		it->second->AddDrawCommand(worldMatrix);
	}
}

void ModelDrawerManager::UpdateAll() {
	// 全モデル描画クラスを更新
	for (auto& modelDrawer : modelDrawers_) {
		modelDrawer.second->Update();
	}
}

void ModelDrawerManager::DrawAll() {
	// 全モデル描画クラスを描画
	for (auto& modelDrawer : modelDrawers_) {
		modelDrawer.second->Draw();
	}
}

void ModelDrawerManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void ModelDrawerManager::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void ModelDrawerManager::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void ModelDrawerManager::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void ModelDrawerManager::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}
