#include "SpriteDrawer.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera2DManager/Camera2DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

using namespace MAGIUtility;
using namespace MAGIMath;

SpriteDrawer::SpriteDrawer(
	DXGI* dxgi,
	DirectXCommand* directXCommand,
	SRVUAVManager* srvUavManager,
	GraphicsPipelineManager* graphicsPipelineManager,
	Camera2DManager* camera2DManager
) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera2DManager(camera2DManager);

}

SpriteDrawer::~SpriteDrawer() {
}

void SpriteDrawer::Update() {

}

void SpriteDrawer::Draw(BlendMode blendMode) {
	blendMode;
}

void SpriteDrawer::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SpriteDrawer::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void SpriteDrawer::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void SpriteDrawer::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void SpriteDrawer::SetCamera2DManager(Camera2DManager* camera2DManager) {
	assert(camera2DManager);
	camera2DManager_ = camera2DManager;
}
