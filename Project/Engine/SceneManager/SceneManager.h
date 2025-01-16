#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <cassert>
#include <type_traits>

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

	// 新しいシーンの追加(テンプレートテンプレート引数)
	template <template <class> class SceneTemplate>
	void AddScene(const std::string& sceneName);

	// シーン変更
	void ChangeScene(const std::string& sceneName);

	// 共有データの取得
	std::shared_ptr<Data> GetData() const;
	// 共有データのセット
	void SetData(const std::shared_ptr<Data>& data);
private:
	// シーン変更処理
	void SwitchScene();
private:
	// シーンファクトリ関数
	using SceneFactoryFunc = std::function<std::unique_ptr<BaseScene<Data>>()>;
	// シーンファクトリ
	std::unordered_map<std::string, SceneFactoryFunc> factory_;
	// 現在シーン
	std::unique_ptr<BaseScene<Data>> currentScene_;
	// 次のシーン
	std::unique_ptr<BaseScene<Data>> nextScene_;
	// 教諭データ
	std::shared_ptr<Data> data_;

};

#include "SceneManager.ipp"