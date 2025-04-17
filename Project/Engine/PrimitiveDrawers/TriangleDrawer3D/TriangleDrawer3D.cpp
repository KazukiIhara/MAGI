#include "TriangleDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIUtility;
using namespace MAGIMath;

TriangleDrawer3D::TriangleDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
	SetDXGI(dxgi);
	SetDirectXCommand(directXCommand);
	SetSRVUAVManager(srvUavManager);
	SetGraphicsPipelineManager(graphicsPipelineManager);
	SetCamera3DManager(camera3DManager);
	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// Materialリソースを作る
	CreateMaterialResource();
	// Materialデータを書き込む
	MapMaterialData();

	triangles_.resize(kNumMaxInstance);
	Logger::Log("TriangleDrawer3D Initialize\n");
}

TriangleDrawer3D::~TriangleDrawer3D() {
	Logger::Log("TriangleDrawer3D Finalize\n");
}

void TriangleDrawer3D::Update() {
	// 上限を超えていたらassert
	assert(currentIndex_ <= kNumMaxInstance);
	instanceCount_ = currentIndex_;
	// インスタンスの数だけコピー
	if (instancingData_ && materialData_ && instanceCount_ > 0) {
		std::memcpy(instancingData_, triangles_.data(), instanceCount_ * sizeof(TriangleData3DForGPU));
	}

	// インデックスリセット
	currentIndex_ = 0; 
}

void TriangleDrawer3D::Draw() {
	// インスタンスがなければスキップ
	if (instanceCount_ == 0) return;

	// コマンドリストを取得
	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	// PSOを設定
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Triangle3D, blendMode_));

	// Cameraを転送（b0）
	camera3DManager_->TransferCurrentCamera(0);

	// TriangleData3DForGPU StructuredBuffer (t0)
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex));

	// 描画
	commandList->DispatchMesh(1, instanceCount_, 1);
}

void TriangleDrawer3D::AddTriangle(const Matrix4x4& worldMatrix, const RGBA& color) {
	// 座標と形状データ
	TriangleData3DForGPU newTriangleData{
		.worldMatrix = worldMatrix,
		.color = RGBAToVector4(color),
	};
	// インデックスに追加
	triangles_[currentIndex_++] = newTriangleData;
}

void TriangleDrawer3D::ClearTriangles() {
	triangles_.clear();
}

void TriangleDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void TriangleDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void TriangleDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void TriangleDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void TriangleDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void TriangleDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(TriangleData3DForGPU) * kNumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex, instancingResource_.Get(), kNumMaxInstance, sizeof(TriangleData3DForGPU));
}

void TriangleDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void TriangleDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * kNumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), kNumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void TriangleDrawer3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// マテリアルのデフォルト値を設定
	uint32_t textureIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		materialData_[i].textureIndex = textureIndex;
		materialData_[i].baseColor = { 1.0f,1.0f,1.0f,1.0f };
		materialData_[i].uvScale = { 1.0f,1.0f };
		materialData_[i].uvRotation = 0.0f;
		materialData_[i].uvTransform = { 0.0f,0.0f };
	}
}
