#pragma once

class DirectXCommand;

class ScissorRect {
public:
	ScissorRect(DirectXCommand* command);
	~ScissorRect();

	// 初期化
	void Initialize(DirectXCommand* command);

	// シザー矩形を設定
	void SettingScissorRect();

private:
	// コマンドのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* command);

private:
	// コマンドのインスタンスを受け取る箱
	DirectXCommand* directXCommand_ = nullptr;

};