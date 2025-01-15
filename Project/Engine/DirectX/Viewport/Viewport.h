#pragma once

class DirectXCommand;

class Viewport {
public:
	Viewport(DirectXCommand* directXCommand);
	~Viewport();

	// 初期化
	void Initialize(DirectXCommand* directXCommand);

	// ビューポートを設定
	void SettingViewport();

private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);

private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;
};