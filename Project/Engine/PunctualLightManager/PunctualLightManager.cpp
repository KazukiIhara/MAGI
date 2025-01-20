#include "PunctualLightManager.h"

#include <cassert>

#include "DirectX/DXGI/DXGI.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"


PunctualLightManager::PunctualLightManager(DXGI* dxgi, SRVUAVManager* srvuavManager) {
	Initialize(dxgi, srvuavManager);
}

PunctualLightManager::~PunctualLightManager() {

}

void PunctualLightManager::Initialize(DXGI* dxgi, SRVUAVManager* srvuavManager) {
	SetSrvUavManager(srvuavManager);

	CreateLightsResource();
	MapLightsData();

	CreateLightCountResource();
	MapLightCountData();

	// Srvの作成
	srvIndex_ = srvuavManager_->Allocate();
	srvuavManager_->CreateSrvStructuredBuffer(srvIndex_, lightsResource_.Get(), kMaxLightNum_, sizeof(PunctualLightData));
}

void PunctualLightManager::Update() {
	// 現在のライトのサイズを取得
	currentLightNum_ = light_.size();

}

void PunctualLightManager::TransferLightsData() {

}

void PunctualLightManager::AddNewLight(const std::string& lightName, const PunctualLightData& lightData) {
	lightName;
	lightData;
}

void PunctualLightManager::SetLightData(const std::string& lightName, const PunctualLightData& lightData) {
	lightName;
	lightData;
}

void PunctualLightManager::CreateLightsResource() {
	lightsResource_ = dxgi_->CreateBufferResource(sizeof(PunctualLightData) * kMaxLightNum_);
}

void PunctualLightManager::MapLightsData() {
	lightsData_ = nullptr;
	lightsResource_->Map(0, nullptr, reinterpret_cast<void**>(lightsData_));

	

}

void PunctualLightManager::CreateLightCountResource() {

}

void PunctualLightManager::MapLightCountData() {

}

void PunctualLightManager::UpdateLightsData() {

}

void PunctualLightManager::UpdateLightCountData() {

}

void PunctualLightManager::SetDXGI(DXGI* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}

void PunctualLightManager::SetSrvUavManager(SRVUAVManager* srvuavManager) {
	assert(srvuavManager);
	srvuavManager_ = srvuavManager;
}
