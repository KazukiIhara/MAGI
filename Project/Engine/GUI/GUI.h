#pragma once

// 前方宣言
class DataIO;
class TextureDataContainer;

// エンジンのグラフィックUIクラス
class GUI {
public:
	GUI(
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
	// DataIOのインスタンス
	DataIO* dataIO_ = nullptr;
	// TextureDataContainerのインスタンス
	TextureDataContainer* textureDataContainer_ = nullptr;
};
