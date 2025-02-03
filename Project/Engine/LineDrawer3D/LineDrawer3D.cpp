#include "LineDrawer3D.h"

#include "Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "Framework/MAGI.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

using namespace MAGIUtility;

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
	// 最大数を超えていたら追加しない
	if (lines_.size() >= kNumMaxInstance) {
		return;
	}
	// 追加するLine
	LineData3D newLineData{};
	newLineData.start = start;
	newLineData.end = end;
	newLineData.color = RGBAToVector4(color);
	// コンテナに挿入
	lines_.push_back(newLineData);
}

void LineDrawer3D::ClearLines() {
	lines_.clear();
}

void LineDrawer3D::Initialize(Camera3DManager* camera3DManager) {
	SetCamera3DManager(camera3DManager);
	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// srvのインデックスを割り当て
	srvIndex_ = MAGISYSTEM::ViewAllocate();
	// Srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kNumMaxInstance, sizeof(LineData3D));
}

void LineDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void LineDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = MAGISYSTEM::CreateBufferResource(sizeof(LineData3D) * kNumMaxInstance);
}

void LineDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
