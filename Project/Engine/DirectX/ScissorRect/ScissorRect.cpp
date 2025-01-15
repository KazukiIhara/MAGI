#include "ScissorRect.h"

#include <cassert>

#include "Logger/Logger.h"

#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "WindowApp/WindowApp.h"

ScissorRect::ScissorRect(DirectXCommand* directXCommand) {
	Initialize(directXCommand);
	Logger::Log("ScissorRect Initialize\n");
}

ScissorRect::~ScissorRect() {
	Logger::Log("ScissorRect Finalize\n");
}

void ScissorRect::Initialize(DirectXCommand* directXCommand) {
	SetDirectXCommand(directXCommand);
}

void ScissorRect::SettingScissorRect() {
	// シザー矩形の設定
	D3D12_RECT scissorRect{};
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WindowApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WindowApp::kClientHeight;
	directXCommand_->GetList()->RSSetScissorRects(1, &scissorRect);
}

void ScissorRect::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
