#pragma once

// 前方宣言
class DeltaTimer;
class SRVUAVManager;
class DataIO;
class TextureDataContainer;

class ColliderManager;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(
		DeltaTimer* deltaTimer,
		SRVUAVManager* srvUavManager,
		DataIO* dataIO,
		TextureDataContainer* textureDataContainer
	);
	void Update();
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();

	// 
	// AssetsContainer
	// 
	void ShowTextureDatas();
	void ShowModelDatas();
	void ShowAnimaionDatas();
	void ShowSoundDatas();

	// 
	// ObjectsManager
	// 
	void ShowColliderManager();


private:
	// DeltaTimerのインスタンス
	DeltaTimer* deltaTimer_ = nullptr;
	// SrvUavManagerのインスタンス
	SRVUAVManager* srvUavManager_ = nullptr;
	// DataIOのインスタンス
	DataIO* dataIO_ = nullptr;
	// TextureDataContainerのインスタンス
	TextureDataContainer* textureDataContainer_ = nullptr;

	// コライダーマネージャのインスタンス
	ColliderManager* colliderManager_ = nullptr;
};
