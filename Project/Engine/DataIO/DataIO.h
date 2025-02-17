#pragma once

// C++
#include <string>

// 前方宣言
class ColliderManager;

/// <summary>
/// データ入出力クラス
/// </summary>
class DataIO {
public:
	DataIO(ColliderManager* colliderManager);
	~DataIO();

	void Initialize();

	// フレーム開始時の処理
	void BeginFrame();
	// フレーム終了時の処理
	void EndFrame();

private:
	// コライダーマネージャのセット
	void SetColliderManager(ColliderManager* colliderManager);
private:


private:
	// 
	// マネージャクラスのポインタ
	// 

	// コライダーマネージャクラス
	ColliderManager* colliderManager_ = nullptr;
};