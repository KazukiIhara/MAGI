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

	// 3D描画オブジェクト
	void ShowRenderer3DManager();
	void ShowRenderer3DDataSaveUI();
	void ShowRenderer3DDataLoadUI();
	void ShowCreateRenderer3DUI();
	void ShowCreateRenderer3DWindow();
	void ShowRenderer3DList(const std::vector<std::unique_ptr<BaseRenderable3D>>& colliders, int& selectedIndex);
	

	// Collider
	void ShowColliderManager();
	void ShowColliderDataSaveUI();
	void ShowColliderDataLoadUI();
	void ShowCreateColliderUI();
	void ShowCreateColliderWindow();
	void ShowColliderList(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex);
	void ShowColliderSetting(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex);
	void ShowColliderInformation(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex);
private:
	// ウィンドウ表示フラグ

	// 3D描画オブジェクト作成ウィンドウの表示管理フラグ
	bool showRenderer3DCreateWindow_ = false;

	// コライダー作成ウィンドウの表示非表示を管理するフラグ
	bool showColliderCreateWindow_ = false;

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
