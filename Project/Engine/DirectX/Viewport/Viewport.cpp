#include "Viewport.h"

#include <cassert>

#include "Logger/Logger.h"

Viewport::Viewport(DirectXCommand* directXCommand) {
	Initialize(directXCommand);
	Logger::Log("Viewport Initialize");
}

Viewport::~Viewport() {
	Logger::Log("Viewport Finalize");
}

void Viewport::Initialize(DirectXCommand* directXCommand) {
	SetDirectXCommand(directXCommand);
}

void Viewport::SettingViewport() {

}

void Viewport::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}
