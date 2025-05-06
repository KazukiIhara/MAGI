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

	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		spheres_[i].resize(PrimitiveCommonConst::NumMaxInstance);
		materials_[i].resize(PrimitiveCommonConst::NumMaxInstance);

		instancingResource_[i] = dxgi_->CreateBufferResource(sizeof(SphereData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		instancingSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(SphereData3DForGPU));
		instancingResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_[i]));

		materialResource_[i] = dxgi_->CreateBufferResource(sizeof(PrimitiveMaterialData3DForGPU) * PrimitiveCommonConst::NumMaxInstance);
		materialSrvIndex_[i] = srvUavManager_->Allocate();
		srvUavManager_->CreateSrvStructuredBuffer(materialSrvIndex_[i], materialResource_[i].Get(), PrimitiveCommonConst::NumMaxInstance, sizeof(PrimitiveMaterialData3DForGPU));
		materialResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_[i]));

		currentIndex_[i] = 0;
		instanceCount_[i] = 0;

		// デフォルトマテリアル
		uint32_t texIndex = MAGISYSTEM::GetTexture()["EngineAssets/Images/uvChecker.png"].srvIndex;
		for (uint32_t j = 0; j < PrimitiveCommonConst::NumMaxInstance; ++j) {
			materialData_[i][j].textureIndex = texIndex;
			materialData_[i][j].baseColor = { 1.0f,1.0f,1.0f,1.0f };
			materialData_[i][j].uvMatrix = MakeIdentityMatrix4x4();
		}
	}


	Logger::Log("SphereDrawer3D Initialize\n");
}

SphereDrawer3D::~SphereDrawer3D() {
	Logger::Log("SphereDrawer3D Finalize\n");
}

void SphereDrawer3D::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; ++i) {
		assert(currentIndex_[i] <= PrimitiveCommonConst::NumMaxInstance);
		instanceCount_[i] = currentIndex_[i];

		if (instanceCount_[i] > 0 && instancingData_[i] && materialData_[i]) {
			std::memcpy(instancingData_[i], spheres_[i].data(), sizeof(SphereData3DForGPU) * instanceCount_[i]);
			std::memcpy(materialData_[i], materials_[i].data(), sizeof(PrimitiveMaterialData3DForGPU) * instanceCount_[i]);
		}

		currentIndex_[i] = 0;
	}
}

void SphereDrawer3D::Draw(BlendMode mode) {
	const uint32_t i = static_cast<uint32_t>(mode);
	if (instanceCount_[i] == 0) return;

	ID3D12GraphicsCommandList6* commandList = directXCommand_->GetList6();
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Sphere3D));
	commandList->SetPipelineState(graphicsPipelineManager_->GetPipelineState(GraphicsPipelineStateType::Sphere3D, mode));
	camera3DManager_->TransferCurrentCamera(0);

	commandList->SetGraphicsRootDescriptorTable(1, srvUavManager_->GetDescriptorHandleGPU(instancingSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(2, srvUavManager_->GetDescriptorHandleGPU(materialSrvIndex_[i]));
	commandList->SetGraphicsRootDescriptorTable(3, srvUavManager_->GetDescriptorHandleGPU(0));

	RootConstants rootConstants{};
	rootConstants.baseInstanceIndex = 0;
	commandList->SetGraphicsRoot32BitConstants(4, 1, &rootConstants, 0);

	commandList->DispatchMesh(1, instanceCount_[i], 1);
}

void SphereDrawer3D::AddSphere(const Matrix4x4& worldMatrix, const SphereData3D& data, const PrimitiveMaterialData3D& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);
	if (currentIndex_[blendIndex] >= PrimitiveCommonConst::NumMaxInstance) {
		Logger::Log("SphereDrawer3D: Max instance count exceeded!\n");
		return;
	}

	SphereData3DForGPU newSphereData{
		.worldMatrix = worldMatrix,
		.worldInverseTranspose = MakeInverseTransposeMatrix(worldMatrix),
		.radius = data.radius,
		.longitudeSegments = data.verticalSegments,
		.latitudeSegments = data.horizontalSegments
	};

	PrimitiveMaterialData3DForGPU newMaterialData{
		.textureIndex = material.textureIndex,
		.baseColor = material.baseColor,
		.uvMatrix = MakeUVMatrix(material.uvScale, material.uvRotate, material.uvTranslate)
	};

	spheres_[blendIndex][currentIndex_[blendIndex]] = newSphereData;
	materials_[blendIndex][currentIndex_[blendIndex]] = newMaterialData;
	currentIndex_[blendIndex]++;
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