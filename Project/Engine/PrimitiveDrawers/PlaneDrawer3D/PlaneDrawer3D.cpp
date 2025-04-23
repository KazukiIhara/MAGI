#include "PlaneDrawer3D.h"

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

PlaneDrawer3D::PlaneDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
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
	planes_.resize(PrimitiveCommonConst::NumMaxInstance);
	materials_.resize(PrimitiveCommonConst::NumMaxInstance);

	Logger::Log("PlaneDrawer3D Initialize\n");
}

PlaneDrawer3D::~PlaneDrawer3D() {
	Logger::Log("PlaneDrawer3D Finalize\n");
}

void PlaneDrawer3D::Update() {
	// 上限を超えていたらassert
	assert(currentIndex_ <= PrimitiveCommonConst::NumMaxInstance);
	instanceCount_ = currentIndex_;
	// データが存在し、描画対象がある場合のみコピー
	if (instancingData_ && materialData_ && instanceCount_ > 0) {
		std::memcpy(instancingData_, planes_.data(), instanceCount_ * sizeof(PlaneData3DForGPU));
		std::memcpy(materialData_, materials_.data(), instanceCount_ * sizeof(PrimitiveMaterialData3DForGPU));
	}
	// 板ポリのコンテナをクリア
	ClearPlanes();
}

void PlaneDrawer3D::Draw() {
	if (instanceCount_ == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Plane3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Plane3D, blendMode_));

	camera3DManager_->TransferCurrentCamera(0); // b0

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_));

	// ルート定数（b1
	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 4, &rootConstants, 0);

	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0)); // Bindless Texture

	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvUavManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	commandList->DispatchMesh(1, instanceCount_, 1);
}

void PlaneDrawer3D::AddPlane(
	const Matrix4x4& worldMatrix,
	const PlaneData3D& data,
	const PrimitiveMaterialData3D& material
) {
	// 座標と形状データ
	PlaneData3DForGPU newPlaneData{
		.worldMatrix = worldMatrix,
		.offsets = {
			Vector4(data.verticesOffsets[0].x, data.verticesOffsets[0].y, data.verticesOffsets[0].z, 1.0f),
			Vector4(data.verticesOffsets[1].x, data.verticesOffsets[1].y, data.verticesOffsets[1].z, 1.0f),
			Vector4(data.verticesOffsets[2].x, data.verticesOffsets[2].y, data.verticesOffsets[2].z, 1.0f),
			Vector4(data.verticesOffsets[3].x, data.verticesOffsets[3].y, data.verticesOffsets[3].z, 1.0f)
			},
	};
	planes_[currentIndex_] = newPlaneData;

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

void PlaneDrawer3D::ClearPlanes() {
	// インデックスリセット
	currentIndex_ = 0;
}

void PlaneDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void PlaneDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void PlaneDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void PlaneDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void PlaneDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void PlaneDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(PlaneData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PlaneData3DForGPU));
}

void PlaneDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void PlaneDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void PlaneDrawer3D::MapMaterialData() {
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
