#include "LineDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

using namespace MAGIUtility;

LineDrawer3D::LineDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	Initialize(dxgi, directXCommand, srvUavManager, graphicsPipelineManager, camera3DManager);
	Logger::Log("LineDrawer3D Initialize\n");
}

LineDrawer3D::~LineDrawer3D() {
	Logger::Log("LineDrawer3D Finalize\n");
}

void LineDrawer3D::Update() {
	// 描画すべきインスタンス数
	instanceCount_ = static_cast<uint32_t>(lines_.size());

	if (instancingData_ != nullptr) {
		for (uint32_t i = 0; i < instanceCount_; ++i) {
			// GPUにデータを転送
			instancingData_[i] = lines_[i];
		}
	}
	// ラインのコンテナをクリア
	ClearLines();
}

void LineDrawer3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// PSOを設定
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Line3D, blendMode_));
	// Cameraを転送
	camera3DManager_->TransferCurrentCamera(0);
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(srvIndex_));
	// 描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(2, instanceCount_, 0, 0);
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

void LineDrawer3D::Initialize(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);
	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// srvのインデックスを割り当て
	srvIndex_ = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kNumMaxInstance, sizeof(LineData3D));
}

void LineDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void LineDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void LineDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void LineDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void LineDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void LineDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(LineData3D) * kNumMaxInstance);
}

void LineDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
