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

	// 最大数分確保
	triangles_.resize(PrimitiveCommonConst::NumMaxInstance);
	materials_.resize(PrimitiveCommonConst::NumMaxInstance);

	// Instancingリソースを作る
	CreateInstancingResource();
	// Instancingデータを書き込む
	MapInstancingData();

	// Materialリソースを作る
	CreateMaterialResource();
	// Materialデータを書き込む
	MapMaterialData();

	Logger::Log("TriangleDrawer3D Initialize\n");
}

TriangleDrawer3D::~TriangleDrawer3D() {
	Logger::Log("TriangleDrawer3D Finalize\n");
}

void TriangleDrawer3D::Update() {
	// 上限を超えていたらassert
	assert(currentIndex_ <= PrimitiveCommonConst::NumMaxInstance);
	instanceCount_ = currentIndex_;
	// インスタンスの数だけコピー
	if (instancingData_ && materialData_ && instanceCount_ > 0) {
		std::memcpy(instancingData_, triangles_.data(), instanceCount_ * sizeof(TriangleData3DForGPU));
		std::memcpy(materialData_, materials_.data(), instanceCount_ * sizeof(PrimitiveMaterialData3DForGPU));
	}
	// 三角形データクリア
	ClearTriangles();
}

void TriangleDrawer3D::Draw() {
	// インスタンスがなければスキップ
	if (instanceCount_ == 0) return;
	// コマンドリストを取得
	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	// ルートシグネイチャを設定
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Triangle3D));
	// PSOを設定
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Triangle3D, blendMode_));
	// Cameraを転送（b0）
	camera3DManager_->TransferCurrentCamera(0);
	// TriangleData3DForGPU StructuredBuffer (t0)
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_));
	// PrimitiveManateriData3DForGPU StructuredBuffer (t1)
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_));

	// ルート定数のセット（b1）
	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0; // 必要に応じて設定
	commandList->SetGraphicsRoot32BitConstants(4, 4, &rootConstants, 0); // b1 → RootParam[4]

	// BindlessTexture用のSRVを設定
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	// BindlessTexture用のDescriptorHeapを設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvUavManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// 描画
	commandList->DispatchMesh(1, instanceCount_, 1);
}

void TriangleDrawer3D::AddTriangle(
	const Matrix4x4& worldMatrix,
	const TriangleData3D& data,
	const PrimitiveMaterialData3D& material
) {

	if (currentIndex_ >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("TriangleDrawer3D: Max instance count exceeded!\n");
		return;
	}

	// 座標と形状データ
	TriangleData3DForGPU newTriangleData{
		.worldMatrix = worldMatrix,
		.Offsets{
			Vector4(data.verticesOffsets[0].x,data.verticesOffsets[0].y,data.verticesOffsets[0].z,1.0f),
			Vector4(data.verticesOffsets[1].x,data.verticesOffsets[1].y,data.verticesOffsets[1].z,1.0f),
			Vector4(data.verticesOffsets[2].x,data.verticesOffsets[2].y,data.verticesOffsets[2].z,1.0f),
		},
	};
	// リストに追加
	triangles_[currentIndex_] = newTriangleData;

	// マテリアルデータ
	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = material.textureIndex,
		.baseColor = RGBAToVector4(material.baseColor),
		.uvMatrix = MakeUVMatrix(material.uvScale,material.uvRotate,material.uvTransform),
	};
	// リストに追加
	materials_[currentIndex_] = newMaterialData;

	// インデックスをインクリメント
	currentIndex_++;

}

void TriangleDrawer3D::ClearTriangles() {
	// インデックスリセット
	currentIndex_ = 0;
	// 中身をクリア
	std::ranges::fill(triangles_, TriangleData3DForGPU{});
	std::ranges::fill(materials_, PrimitiveMaterialData3DForGPU{});
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
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(TriangleData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex_ = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(TriangleData3DForGPU));
}

void TriangleDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void TriangleDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void TriangleDrawer3D::MapMaterialData() {
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
