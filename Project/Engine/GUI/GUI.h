#pragma once

// 前方宣言
class ImGuiController;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(ImGuiController* imguiController);
	void Initialize(ImGuiController* imguiController);
	void Update();
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();
private:
	void SetImGuiController(ImGuiController* imguiController);
private:
	// ImGuiControllerのインスタンス
	ImGuiController* imguiController_ = nullptr;
};
