#include "ModelDrawer.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

ModelDrawer::ModelDrawer(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera3DManager* camera3DManager,
	const ModelData& modelData
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);

	modelData;
}

ModelDrawer::~ModelDrawer() {

}

void ModelDrawer::AddDrawCommand(const Matrix4x4& worldMatrix) {
	worldMatrix;
}

void ModelDrawer::Update() {

}

void ModelDrawer::Draw() {

}

void ModelDrawer::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void ModelDrawer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;

}

void ModelDrawer::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void ModelDrawer::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void ModelDrawer::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}
