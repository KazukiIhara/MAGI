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
class IScene {
public:
	// コンストラクタで "共有データ" と "シーンマネージャ" を受け取る
	IScene(std::shared_ptr<Data> data, SceneManager<Data>* sceneManager)
		: data_(data), sceneManager_(sceneManager) {}

	virtual ~IScene() = default;

	// 毎フレームの更新
	virtual void Update() = 0;
	// 毎フレームの描画
	virtual void Draw() = 0;

	// 初期化
	virtual void Initialize() = 0;
	// 終了
	virtual void Finalize() = 0;

protected:
	std::shared_ptr<Data> data_;				// 全シーンで共有するデータ
	SceneManager<Data>* const sceneManager_;	// シーンマネージャへのポインタ
};