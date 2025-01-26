#include "AnimationDataContainer.h"

#include <iostream>
#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"

using namespace MAGIMath;

AnimationDataContainer::AnimationDataContainer() {

	Logger::Log("AnimationDataContainer Initialize\n");
}

AnimationDataContainer::~AnimationDataContainer() {
	Logger::Log("AnimationDataContainer Finalize\n");
}

void AnimationDataContainer::Initialize() {
	
}

void AnimationDataContainer::Load(const std::string& animationFileName, bool isInSameDirectoryAsModel) {

}

AnimationData AnimationDataContainer::FindAnimationData(const std::string& animationName) {
	return AnimationData();
}

AnimationData AnimationDataContainer::LoadAnimation(const std::string& animationFileName, bool isInSameDirectoryAsModel) {
	return AnimationData();
}
