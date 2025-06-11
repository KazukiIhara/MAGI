#include "TransformManager.h"

#include <cassert>

#include "Logger/Logger.h"

TransformManager::TransformManager() {
	Clear();
	Logger::Log("TransformManager Initialized\n");
}

TransformManager::~TransformManager() {
	Clear();
	Logger::Log("TransformManager Finalized\n");
}

std::weak_ptr<Transform3D> TransformManager::Add(std::shared_ptr<Transform3D> transform) {
	assert(transform && "Transform must not be null");
	transforms_.push_back(std::move(transform));
	return transforms_.back();
}

void TransformManager::Update() {
	for (auto& transform : transforms_) {
		if (!transform->GetParent()) {
			transform->Update();
		}
	}
}

void TransformManager::DeleteGarbage() {
	// 生存フラグが消えているトランスフォームの後始末関数を呼ぶ
	for (auto& transform : transforms_) {
		if (transform && !transform->GetisAlive()) {
			transform->Finalize();
		}
	}
	// 生存フラグが消えているトランスフォームを削除
	std::erase_if(transforms_, [](const std::shared_ptr<Transform3D>& transform) {
		return transform && !transform->GetisAlive();
		});
}

void TransformManager::Clear() {
	for (auto& transform : transforms_) {
		if (transform) {
			transform->Finalize();
		}
	}
	transforms_.clear();
}