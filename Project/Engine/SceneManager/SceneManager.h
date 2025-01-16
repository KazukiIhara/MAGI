#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <cassert>
#include "BaseScene/BaseScene.h"
#include "GameData/GameData.h"


template <typename Data>
class SceneManager {
public:
	SceneManager();
	~SceneManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 「template <template <class> class SceneTemplate>」で受け取る
	template <template <class> class SceneTemplate>
	void AddScene(const std::string& sceneName);

	// シーン変更
	void ChangeScene(const std::string& sceneName);

	// 共有データへのアクセサ
	std::shared_ptr<Data> GetData() const {
		return data_;
	}
	void SetData(const std::shared_ptr<Data>& data) {
		data_ = data;
	}
private:
	// シーン変更処理
	void SwitchScene();
private:
	using SceneFactoryFunc = std::function<std::unique_ptr<BaseScene<Data>>()>;

	std::unordered_map<std::string, SceneFactoryFunc> factory_;

	std::unique_ptr<BaseScene<Data>> currentScene_;
	std::unique_ptr<BaseScene<Data>> nextScene_;

	std::shared_ptr<Data> data_;

};