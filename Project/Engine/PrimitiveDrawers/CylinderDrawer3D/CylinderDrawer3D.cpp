#include "CylinderDrawer3D.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "MAGIUitility/MAGIUtility.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

CylinderDrawer3D::CylinderDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
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

	// 最大数分確保
	cylinders_.resize(PrimitiveCommonConst::NumMaxInstance);
	materials_.resize(PrimitiveCommonConst::NumMaxInstance);

	Logger::Log("CylinderDrawer3D Initialize\n");
}

CylinderDrawer3D::~CylinderDrawer3D() {
	Logger::Log("CylinderDrawer3D Finalize\n");
}

void CylinderDrawer3D::Update() {
	// 上限を超えていたらassert
	assert(currentIndex_ <= PrimitiveCommonConst::NumMaxInstance);
	instanceCount_ = currentIndex_;
	// データが存在し、描画対象がある場合のみコピー
	if (instancingData_ && materialData_ && instanceCount_ > 0) {
		std::memcpy(instancingData_, cylinders_.data(), instanceCount_ * sizeof(CylinderData3DForGPU));
		std::memcpy(materialData_, materials_.data(), instanceCount_ * sizeof(PrimitiveMaterialData3DForGPU));
	}
	// シリンダーのコンテナをクリア
	ClearCylinders();
}

void CylinderDrawer3D::Draw() {
	if (instanceCount_ == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Cylinder3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Cylinder3D, blendMode_));

	// カメラ（b0: index 0）
	camera3DManager_->TransferCurrentCamera(0);

	// Descriptor Table 設定
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0)); // Bindless

	// ディスクリプタヒープ設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvUavManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// AS用 baseInstanceIndex（b1: index 4）
	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	// Amplification Shader を使ってインスタンス数ぶん Dispatch
	commandList->DispatchMesh(1, instanceCount_, 1);
}

void CylinderDrawer3D::ClearCylinders() {
	// インデックスリセット
	currentIndex_ = 0;
	// 中身をクリア
	std::ranges::fill(cylinders_, CylinderData3DForGPU{});
	std::ranges::fill(materials_, PrimitiveMaterialData3DForGPU{});
}

void CylinderDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void CylinderDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void CylinderDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void CylinderDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void CylinderDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void CylinderDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(CylinderData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(CylinderData3DForGPU));
}

void CylinderDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void CylinderDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void CylinderDrawer3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// マテリアルのデフォルト値を設定
	uint32_t textureIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
	for (uint32_t i = 0; i < PrimitiveCommonConst::NumMaxInstance; ++i) {
		materialData_[i].textureIndex = textureIndex;
		materialData_[i].baseColor = { 1.0f,1.0f,1.0f,1.0f };
		materialData_[i].uvMatrix = MakeIdentityMatrix4x4();
	}
}
