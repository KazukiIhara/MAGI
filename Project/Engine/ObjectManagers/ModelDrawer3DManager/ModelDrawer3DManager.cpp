#include "ModelDrawer3DManager.h"

ModelDrawer3DManager::ModelDrawer3DManager(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {}

ModelDrawer3DManager::~ModelDrawer3DManager() {}

void ModelDrawer3DManager::CreateModelDrawer(const std::string& modelName, const ModelData& modelData) {}

void ModelDrawer3DManager::DrawModel(const std::string& modelName, const Matrix4x4& worldMatrix) {}

void ModelDrawer3DManager::UpdateAll() {}

void ModelDrawer3DManager::DrawAll() {}
