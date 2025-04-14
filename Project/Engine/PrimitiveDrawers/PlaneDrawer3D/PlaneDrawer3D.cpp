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

	planes_.reserve(kNumMaxInstance);
	Logger::Log("PlaneDrawer3D Initialize\n");
}

PlaneDrawer3D::~PlaneDrawer3D() {
	Logger::Log("PlaneDrawer3D Finalize\n");
}

void PlaneDrawer3D::Update() {
	// 最大数を超えていたら止める
	if (planes_.size() > kNumMaxInstance) {
		assert(false && "Plane size is over !");
	}

	// 描画すべきインスタンス数
	instanceCount_ = static_cast<uint32_t>(planes_.size());

	if (instancingData_ != nullptr && !planes_.empty()) {
		// コピー
		std::memcpy(instancingData_, planes_.data(), instanceCount_ * sizeof(PlaneData3D));
	}
	// 板ポリのコンテナをクリア
	ClearPlanes();
}

void PlaneDrawer3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// PSOを設定
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Plane3D, blendMode_));
	// Cameraを転送
	camera3DManager_->TransferCurrentCamera(0);

	// PlaneData3DStructuredBufferのSRVを設定
	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(planeSrvIndex));
	// MaterialDataStructuredBufferのSRVを設定
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_));
	// BindlessTexture用のSRVを設定
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	// BindlessTexture用のDescriptorHeapを設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvUavManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// 描画
	commandList->DrawInstanced(4, instanceCount_, 0, 0);
}

void PlaneDrawer3D::AddPlane(const WorldTransform& worldTransform, const Vector3& leftTop, const Vector3& rightTop, const Vector3& leftBottom, const Vector3& rightBottom, const RGBA& color) {
	PlaneData3D newPlaneData{};
	newPlaneData.worldMatrix = worldTransform.worldMatrix_;
	newPlaneData.worldInverseTranspose = MakeInverseTransposeMatrix(worldTransform.worldMatrix_);
	newPlaneData.offsets[0] = Vector4(leftTop.x, leftTop.y, leftTop.z, 1.0f);
	newPlaneData.offsets[1] = Vector4(rightTop.x, rightTop.y, rightTop.z, 1.0f);
	newPlaneData.offsets[2] = Vector4(leftBottom.x, leftBottom.y, leftBottom.z, 1.0f);
	newPlaneData.offsets[3] = Vector4(rightBottom.x, rightBottom.y, rightBottom.z, 1.0f);
	newPlaneData.color = RGBAToVector4(color);

	planes_.push_back(newPlaneData);
}

void PlaneDrawer3D::ClearPlanes() {
	planes_.clear();
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
	instancingResource_ = dxgi_->CreateBufferResource(sizeof(PlaneData3D) * kNumMaxInstance);
	// srvのインデックスを割り当て
	planeSrvIndex = srvUavManager_->Allocate();
	// Srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(planeSrvIndex, instancingResource_.Get(), kNumMaxInstance, sizeof(PlaneData3D));
}

void PlaneDrawer3D::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
}

void PlaneDrawer3D::CreateMaterialResource() {
	// Material用のリソースを作る
	materialResource_ = dxgi_->CreateBufferResource(sizeof(PlaneMaterialData3D) * kNumMaxInstance);
	// srvのインデックスを割り当て
	materialSrvIndex_ = srvUavManager_->Allocate();
	// srvを作成
	srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_, materialResource_.Get(), kNumMaxInstance, sizeof(PlaneMaterialData3D));
}

void PlaneDrawer3D::MapMaterialData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	// 全Planeにデフォルトテクスチャを指定
	uint32_t textureIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		materialData_[i].textureIndex = textureIndex;
	}
}
