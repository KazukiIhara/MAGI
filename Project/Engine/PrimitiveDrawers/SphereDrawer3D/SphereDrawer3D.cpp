#include "SphereDrawer3D.h"

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

SphereDrawer3D::SphereDrawer3D(DXGI* dxgi, DirectXCommand* directXCommand, SRVUAVManager* srvUavManager, GraphicsPipelineManager* graphicsPipelineManager, Camera3DManager* camera3DManager) {
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

	spheres_.reserve(PrimitiveCommonConst::NumMaxInstance);
	Logger::Log("SphereDrawer3D Initialize\n");
}

SphereDrawer3D::~SphereDrawer3D() {
	Logger::Log("SphereDrawer3D Finalize\n");
}

void SphereDrawer3D::Update() {
	// 最大数を超えていたら止める
	assert(spheres_.size() <= PrimitiveCommonConst::NumMaxInstance && "Sphere size is over!");

	// 描画すべきインスタンス数
	instanceCount_ = static_cast<uint32_t>(spheres_.size());

	// データが存在し、描画対象がある場合のみコピー
	if (instancingData_ != nullptr && materialData_ != nullptr && instanceCount_ > 0) {
		std::memcpy(instancingData_, spheres_.data(), instanceCount_ * sizeof(SphereData3DForGPU));
		std::memcpy(materialData_, materials_.data(), instanceCount_ * sizeof(PrimitiveMaterialData3DForGPU));
	}
	// 球体のコンテナをクリア
	ClearSpheres();
}

void SphereDrawer3D::Draw() {
	if (instanceCount_ == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();

	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Sphere3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Sphere3D, blendMode_));
	camera3DManager_->TransferCurrentCamera(0);

	// Descriptor Table 設定
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvUavManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// AS用 baseInstanceIndex（b1: index 4）
	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	// MS用 baseInstanceIndex（b2: index 5）
	commandList->SetGraphicsRoot32BitConstant(5, 0, 0);

	// ← ここがポイント！
	commandList->DispatchMesh(1, instanceCount_, 1); // ← Y方向に instanceCount_ 分だけ dispatch
}


void SphereDrawer3D::AddSphere(
	const Matrix4x4& worldMatrix,
	const SphereData3D& data,
	const PrimitiveMaterialData3D& material
) {

	// 座標と形状データ
	SphereData3DForGPU newSphereData{
		.worldMatrix = worldMatrix,
		.radius = data.radius,
		.longitudeSegments = data.verticalSegments,
		.latitudeSegments = data.horizontalSegments
	};
	spheres_.push_back(newSphereData);

	// マテリアルデータ
	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = material.textureIndex,
		.baseColor = RGBAToVector4(material.baseColor),
		.uvMatrix = MakeUVMatrix(material.uvScale,material.uvRotate,material.uvTransform),
	};
	materials_.push_back(newMaterialData);
}

void SphereDrawer3D::ClearSpheres() {
	spheres_.clear();
}

void SphereDrawer3D::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void SphereDrawer3D::SetDirectXCommand(DirectXCommand* directXCommand) {
	assert(directXCommand);
	directXCommand_ = directXCommand;
}

void SphereDrawer3D::SetSRVUAVManager(SRVUAVManager* srvUavManager) {
	assert(srvUavManager);
	srvUavManager_ = srvUavManager;
}

void SphereDrawer3D::SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager) {
	assert(graphicsPipelineManager);
	graphicsPipelineManager_ = graphicsPipelineManager;
}

void SphereDrawer3D::SetCamera3DManager(Camera3DManager* camera3DManager) {
	assert(camera3DManager);
	camera3DManager_ = camera3DManager;
}

void SphereDrawer3D::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(SphereData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	instancingSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex, instancingResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(SphereData3DForGPU));
}

void SphereDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void SphereDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
}

void SphereDrawer3D::MapMaterialData() {
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
