#include "Viewport.h"

#include <cassert>

#include "Logger/Logger.h"
#include "WindowApp/WindowApp.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"

Viewport::Viewport(DirectXCommand* directXCommand) {
	Initialize(directXCommand);
	Logger::Log("Viewport Initialize\n");
}

Viewport::~Viewport() {
	Logger::Log("Viewport Finalize\n");
}

void Viewport::Initialize(DirectXCommand* directXCommand) {
	SetDirectXCommand(directXCommand);
}

void Viewport::SettingViewport() {
	// ビューポートの設定
	D3D12_VIEWPORT viewport{};
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WindowApp::kClientWidth;
	viewport.Height = WindowApp::kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	directXCommand_->GetList()->RSSetViewports(1, &viewport);
}

void Viewport::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
