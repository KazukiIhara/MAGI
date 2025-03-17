#pragma once

// C++
#include <vector>
#include <memory>

// 前方宣言
class DeltaTimer;
class SRVUAVManager;
class DataIO;
class TextureDataContainer;

class Renderer3DManager;
class ColliderManager;

class BaseRenderable3D;
class BaseCollider3D;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(
		DeltaTimer* deltaTimer,
		SRVUAVManager* srvUavManager,
		DataIO* dataIO,
		TextureDataContainer* textureDataContainer
	);

	// メインUI描画
	void ShowMainUI();

	//
	// アクセッサ
	//

	// エンジンウィンドウの表示フラグ
	bool GetIsShowEngineWindow()const;
	void SetIsShowEngineWindow(bool isShowWindow);
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();

private:
	// エンジンのウィンドウを描画するフラグ
	bool isShowEngineWindow_ = false;

private:
	// DeltaTimerのインスタンス
	DeltaTimer* deltaTimer_ = nullptr;
	// SrvUavManagerのインスタンス
	SRVUAVManager* srvUavManager_ = nullptr;
	// DataIOのインスタンス
	DataIO* dataIO_ = nullptr;
	// TextureDataContainerのインスタンス
	TextureDataContainer* textureDataContainer_ = nullptr;

	// 3D描画マネージャのインスタンス
	Renderer3DManager* renderer3DManager_ = nullptr;
	// コライダーマネージャのインスタンス
	ColliderManager* colliderManager_ = nullptr;
};
