#include "SkeletonDataContainer.h"

#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"

SkeletonDataContainer::SkeletonDataContainer() {
	Initialize();
	Logger::Log("SkeletonDataContainer Initialize\n");
}

SkeletonDataContainer::~SkeletonDataContainer() {
	Logger::Log("SkeletonDataContainer Finalize\n");
}

void SkeletonDataContainer::Initialize() {
	skeletonDatas_.clear();
}

void SkeletonDataContainer::Load(const std::string& skeletonName) {
	// 読み込み済みモデルを検索
	if (skeletonDatas_.contains(skeletonName)) {
		// 読み込み済みなら早期リターン
		return;
	}
	// モデルを読み込みコンテナに挿入
	skeletonDatas_.insert(std::make_pair(skeletonName, LoadSkeleton(skeletonName)));
}

SkeletonData SkeletonDataContainer::LoadSkeleton(const std::string& skeletonName) {
	return SkeletonData();
}
