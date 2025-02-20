#pragma once

// C++
#include <string>
#include <vector>
#include <memory>

// 前方宣言
class BaseCollider3D;
class Renderer3DManager;
class ColliderManager;

/// <summary>
/// データ入出力クラス
/// </summary>
class DataIO {
public:
	DataIO(Renderer3DManager* renderer3DManager, ColliderManager* colliderManager);
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

	// 3D描画オブジェクトデータファイルの読み込み
	void LoadRenderer3DDataFile(const std::string& fileName);

	// コライダーデータファイルの読み込み
	void LoadColliderDataFile(const std::string& fileName);

	// 
	// ファイルセーブ関数
	// 

	// 3D描画オブジェクトデータファイルのセーブ
	void SaveRenderer3DDataFile(const std::string& fileName);

	// コライダーデータファイルのセーブ
	void SaveColliderDataFile(const std::string& fileName);

	// 
	// 所有しているマネージャのインスタンスを送る
	// 

	// 3D描画マネージャ
	Renderer3DManager* GetRenderer3DManager();
	// コライダーマネージャ
	ColliderManager* GetColliderManager();

private:


private:
	// 3D描画マネージャのセット
	void SetRenderer3DManager(Renderer3DManager* renderer3DManager);
	// コライダーマネージャのセット
	void SetColliderManager(ColliderManager* colliderManager);
private:
	// 
	// マネージャクラスのポインタ
	// 

	// 3D描画マネージャクラス
	Renderer3DManager* renderer3DManager_ = nullptr;
	// コライダーマネージャクラス
	ColliderManager* colliderManager_ = nullptr;
};