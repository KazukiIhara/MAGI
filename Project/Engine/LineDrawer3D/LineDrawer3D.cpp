#include "LineDrawer3D.h"

#include "Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"

#include <cassert>

LineDrawer3D::LineDrawer3D(Camera3DManager* camera3DManager) {
	Initialize(camera3DManager);
	Logger::Log("LineDrawer3D Initialize\n");
}

LineDrawer3D::~LineDrawer3D() {
	Logger::Log("LineDrawer3D Finalize\n");
}

void LineDrawer3D::Update() {

}

void LineDrawer3D::Draw() {

}

void LineDrawer3D::AddLine(const Vector3& start, const Vector3& end, const RGBA& color) {
	start;
	end;
	color;
}

void LineDrawer3D::ClearLines() {

}

void LineDrawer3D::Initialize(Camera3DManager* camera3DManager) {
	SetCamera3DManager(camera3DManager);
}

void LineDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void LineDrawer3D::CreateInstancingResource() {

}

void LineDrawer3D::MapInstancingData() {

}
