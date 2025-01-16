#pragma once
#include <memory>

// 前方宣言
template <typename Data>
class SceneManager;

template <typename Data>
class BaseScene {
public:
	BaseScene(std::shared_ptr<Data> data, SceneManager<Data>* sceneManager)
		: data_(data), sceneManager_(sceneManager) {}

	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Finalize() {}

protected:
	// 共有データ
	std::shared_ptr<Data> data_;
	// シーンマネージャのポインタ
	SceneManager<Data>* const sceneManager_;
};