#include "SceneManager/SceneManager.h"


template <typename Data>
SceneManager<Data>::SceneManager()
	: data_(std::make_shared<Data>()) {}

template <typename Data>
SceneManager<Data>::~SceneManager() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
}

template <typename Data>
void SceneManager<Data>::Update() {
	SwitchScene();
	if (currentScene_) {
		currentScene_->Update();
	}
}

template <typename Data>
void SceneManager<Data>::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}

template <typename Data>
template <template <class> class SceneTemplate>
void SceneManager<Data>::AddScene(const std::string& sceneName) {
	// SceneTemplate が「template <class> class」＝ Data を受け取るクラステンプレート前提
	// ここで実際に「SceneTemplate<Data>」という具体型をエイリアスとして定義
	using SceneType = SceneTemplate<Data>;

	// BaseScene<Data> を継承しているかをチェック
	static_assert(std::is_base_of<BaseScene<Data>, SceneType>::value,
		"SceneTemplate<Data> must derive from BaseScene<Data>.");

	// ファクトリ関数を登録
	factory_[sceneName] = [this]() {
		return std::make_unique<SceneType>(this->data_, this);
		};
}

template <typename Data>
void SceneManager<Data>::ChangeScene(const std::string& sceneName) {
	auto it = factory_.find(sceneName);
	assert(it != factory_.end() && "No scene found with the given name");

	nextScene_ = it->second();
}

template <typename Data>
void SceneManager<Data>::SwitchScene() {
	if (nextScene_) {
		if (currentScene_) {
			currentScene_->Finalize();
			currentScene_.reset();
		}
		currentScene_ = std::move(nextScene_);
		currentScene_->Initialize();
	}
}

// "SceneManager<GameData>" の実体を生成させるための明示的インスタンシエーション
template class SceneManager<GameData>;