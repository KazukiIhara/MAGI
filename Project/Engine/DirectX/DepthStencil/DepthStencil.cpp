#include "DepthStencil.h"

#include <cassert>

#include "Logger/Logger.h"

#include "WindowApp/WindowApp.h"
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/DSVManager/DSVManager.h"

DepthStencil::DepthStencil(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager) {
	Initialize(dxgi, command, dsvManager);
	Logger::Log("DepthStencil Initialize");
}

DepthStencil::~DepthStencil() {
	Logger::Log("DepthStencil Finalize");
}

void DepthStencil::Initialize(DXGI* dxgi, DirectXCommand* command, DSVManager* dsvManager) {
	// DXGIのセット
	SetDXGI(dxgi);
	// コマンドをセット
	SetCommand(command);
	// DSVマネージャをセット
	SetDSVManager(dsvManager);
	// DepthStencilViewを作成
	CreateDepthStencilView();
}

void DepthStencil::ClearDepthView() {
	// 指定した深度で画面全体をクリア
	directXCommand_->GetList()->ClearDepthStencilView(
		dsvManager_->GetDescriptorHandleCPU(dsvIndex_),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE DepthStencil::GetDepthStencilResorceCPUHandle() {
	return dsvManager_->GetDescriptorHandleCPU(dsvIndex_);
}

void DepthStencil::CreateDepthStencilView() {
	// DepthStencilTextureをウィンドウのサイズで作成
	depthStencilResource_ = dxgi_->CreateDepthStencilTextureResource(
		WindowApp::kClientWidth,
		WindowApp::kClientHeight
	);
	// インデックス割り当て
	dsvIndex_ = dsvManager_->Allocate();
	// dsv作成
	dsvManager_->CreateDSVTexture2d(
		dsvIndex_,
		depthStencilResource_.Get(),
		DXGI_FORMAT_D24_UNORM_S8_UINT
	);
}

void DepthStencil::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void DepthStencil::SetCommand(DirectXCommand* command) {
	assert(command);
	directXCommand_ = command;
}

void DepthStencil::SetDSVManager(DSVManager* dsvManager) {
	assert(dsvManager);
	dsvManager_ = dsvManager;
}
