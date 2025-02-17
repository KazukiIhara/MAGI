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

	// 初期化
	void Initialize();

	// フレーム開始時の処理
	void BeginFrame();
	// フレーム終了時の処理
	void EndFrame();

	// 
	// ファイル読み込み関数
	// 

	// コライダーデータファイルの読み込み
	void LoadColliderDataFile(const std::string& fileName);

	// 
	// ファイルセーブ関数
	// 

	// コライダーデータファイルのセーブ
	void SaveColliderDataFile(const std::string& fileName);

	// 
	// GUIクラスで描画する用にマネージャのインスタンスを送るクラス
	// 

	// コライダーマネージャ
	ColliderManager* GetColliderManager();

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