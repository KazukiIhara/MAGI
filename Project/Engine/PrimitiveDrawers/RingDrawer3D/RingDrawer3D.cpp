#include "RingDrawer3D.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"

#include "Logger/Logger.h"
#include "Const/Primitive3DConst.h"

// TODO::フレームワーク内にあるクラスはMAGIをインクルードしてはいけない
#include "Framework/MAGI.h"

using namespace MAGIMath;

RingDrawer3D::RingDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
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
	rings_.resize(PrimitiveCommonConst::NumMaxInstance);
	materials_.resize(PrimitiveCommonConst::NumMaxInstance);

	Logger::Log("RingDrawer3D Initialize\n");
}

RingDrawer3D::~RingDrawer3D() {
	Logger::Log("RingDrawer3D Finalize\n");
}

void RingDrawer3D::Update() {
	// 上限を超えていたらassert
	assert(currentIndex_ <= PrimitiveCommonConst::NumMaxInstance);
	instanceCount_ = currentIndex_;
	// データが存在し、描画対象がある場合のみコピー
	if (instancingData_ && materialData_ && instanceCount_ > 0) {
		std::memcpy(instancingData_, rings_.data(), instanceCount_ * sizeof(RingData3DForGPU));
		std::memcpy(materialData_, materials_.data(), instanceCount_ * sizeof(PrimitiveMaterialData3DForGPU));
	}
	// リングのコンテナをクリア
	ClearRings();
}

void RingDrawer3D::Draw() {
	if (instanceCount_ == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Ring3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Ring3D, blendMode_));

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

void RingDrawer3D::AddRing(
	const Matrix4x4& worldMatrix,
	const RingData3D& data,
	const PrimitiveMaterialData3D& material
) {
	if (currentIndex_ >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("RingDrawer3D: Max instance count exceeded!\n");
		return;
	}

	// 座標と形状データ
	RingData3DForGPU newRingData{
		.worldMatrix = worldMatrix,
		.ringDivide = data.ringDivide,
		.outerRadius = data.outerRadius,
		.innerRadius = data.innerRadius,
		.radianPerDivide = 2.0f * std::numbers::pi_v<float> / static_cast<float>(data.ringDivide),
	};
	rings_[currentIndex_] = newRingData;

	// マテリアルデータ
	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = material.textureIndex,
		.baseColor = material.baseColor,
		.uvMatrix = MakeUVMatrix(material.uvScale,material.uvRotate,material.uvTranslate),
	};
	materials_[currentIndex_] = newMaterialData;

	// インデックスをインクリメント
	currentIndex_++;
}

void RingDrawer3D::ClearRings() {
	// インデックスリセット
	currentIndex_ = 0;
}

void RingDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void RingDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void RingDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void RingDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void RingDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void RingDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(RingData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(RingData3DForGPU));
}

void RingDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void RingDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void RingDrawer3D::MapMaterialData() {
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
