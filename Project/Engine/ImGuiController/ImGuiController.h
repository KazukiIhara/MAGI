#pragma once

// WindowsAPI
#include <Windows.h>

// C++
#include <cstdint>

// imgui
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

// 前方宣言
class WindowApp;
class DXGI;
class DirectXCommand;
class SRVUAVManager;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
);

class ImGuiController {
public:
	ImGuiController(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvManager);
	~ImGuiController();

	// 初期化
	void Initialize(WindowApp* windowApp, DXGI* dxgi, DirectXCommand* command, SRVUAVManager* srvUavManager);

	// フォント設定
	void FontSetting();

	// 更新処理の先頭に呼び出す処理
	void BeginFrame();

	// 更新処理の最後に呼び出す処理
	void EndFrame();

	// 描画
	void Draw();

	// ImGuiの終了処理
	void Finalize();

	// ImGuiの配色をダークテーマにする
	void SetImGuiDarkTheme();

private:
	void SetWindowApp(WindowApp* windowApp);
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* command);
	void SetSrvUavManager(SRVUAVManager* srvManager);
private: // インスタンスを受け取る変数
	// WinAPP
	WindowApp* windowApp_ = nullptr;
	// dxgi
	DXGI* dxgi_ = nullptr;
	// command
	DirectXCommand* command_ = nullptr;
	// SrvManager
	SRVUAVManager* srvUavManager_ = nullptr;
};