#pragma once

// 前方宣言
class DeltaTimer;
class SRVUAVManager;
class DataIO;
class TextureDataContainer;

class Renderer3DManager;
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

	// 3D描画マネージャのインスタンス
	Renderer3DManager* renderer3DManager_ = nullptr;
	// コライダーマネージャのインスタンス
	ColliderManager* colliderManager_ = nullptr;
};
