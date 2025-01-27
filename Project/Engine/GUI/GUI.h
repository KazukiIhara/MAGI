#pragma once

// 前方宣言
class ImGuiController;
class TextureDataContainer;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(ImGuiController* imguiController,
		TextureDataContainer* textureDataContainer
	);

	void Update();
	void Draw();
private:
	// FPS表示
	void ShowFPS();
	// DeltaTime表示
	void ShowDeltaTime();
private:
	// ImGuiControllerのインスタンス
	ImGuiController* imguiController_ = nullptr;
	// TextureDataContainerのインスタンス
	TextureDataContainer* textureDataContainer_ = nullptr;
};
