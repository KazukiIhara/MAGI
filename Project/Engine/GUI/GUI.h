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
	void Update();
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();

private:


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
