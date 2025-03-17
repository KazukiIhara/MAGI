#pragma once

// C++
#include <vector>
#include <memory>

// ImGui
#include "imgui/imgui.h"

// 前方宣言
class DeltaTimer;
class DataIO;
class SRVUAVManager;
class OffScreenRenderer;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(
		DeltaTimer* deltaTimer,
		SRVUAVManager* srvUavManager,
		DataIO* dataIO,
		OffScreenRenderer* offScreenRenderer
	);

	// メインUI描画
	void ShowMainUI();

	//
	// アクセッサ
	//

	// エンジンウィンドウの表示フラグ
	bool& GetIsShowEngineWindow();
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();

	// シーンウィンドウ描画
	void ShowSceneWindow();

	// ヒエラルキー描画
	void ShowHierarchyWindow();

	// インスペクター描画
	void ShowInspectorWindow();

	// プロジェクトファイル描画
	void ShowProjectWindow();

private:
	// エンジンのウィンドウを描画するフラグ
	bool isShowEngineWindow_ = false;

private:
	// 
	// UI変数
	// 

	// シーンウィンドウ
	ImVec2 sceneTextureSize_ = { 1024.0f,576.0f };
private:
	// DeltaTimerのインスタンス
	DeltaTimer* deltaTimer_ = nullptr;
	// SrvUavManagerのインスタンス
	SRVUAVManager* srvUavManager_ = nullptr;
	// DataIOのインスタンス
	DataIO* dataIO_ = nullptr;
	// OffScreenRendererのインスタンス
	OffScreenRenderer* offScreenRenderer_ = nullptr;
};
