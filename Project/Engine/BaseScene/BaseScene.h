#pragma once

// C++
#include <memory>

// 前方宣言
template <typename Data>
class SceneManager;

/// <summary>
/// すべてのシーンの基底クラス
/// </summary>
template <typename Data>
class BaseScene {
public:
	// コンストラクタで "共有データ" と "シーンマネージャ" を受け取る
	BaseScene(std::shared_ptr<Data> data, SceneManager<Data>* sceneManager)
		: data_(data), sceneManager_(sceneManager) {}

	virtual ~BaseScene() = default;

	// 毎フレームの更新
	virtual void Update() {};
	// 毎フレームの描画
	virtual void Draw() {};

	// 初期化
	virtual void Initialize() {}
	// 終了
	virtual void Finalize() {}

protected:
	std::shared_ptr<Data> data_;				// 全シーンで共有するデータ
	SceneManager<Data>* const sceneManager_;	// シーンマネージャへのポインタ
};