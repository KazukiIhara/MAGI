#include "DataIO.h"

#include "ColliderManager/ColliderManager.h"

#include "Logger/Logger.h"

#include <cassert>

DataIO::DataIO(ColliderManager* colliderManager) {
	// 
	// インスタンスのセット
	// 

	SetColliderManager(colliderManager);

	Initialize();
	Logger::Log("DataIO Initialize\n");
}

DataIO::~DataIO() {
	Logger::Log("DataIO Finalize\n");
}

void DataIO::Initialize() {

}

void DataIO::BeginFrame() {

}

void DataIO::EndFrame() {

}

void DataIO::SetColliderManager(ColliderManager* colliderManager) {
	assert(colliderManager);
	colliderManager_ = colliderManager;
}
