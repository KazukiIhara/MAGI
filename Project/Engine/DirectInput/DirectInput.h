#pragma once

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <cstdint>

#include "ComPtr/ComPtr.h"
#include "Enums/MouseInputEnum.h"

// 前方宣言
class WindowApp;

// DirectInputクラス
class DirectInput {
public:
	DirectInput(WindowApp* windowApp);
	~DirectInput();
	// 初期化
	void Initialize(WindowApp* windowApp);
	// 更新
	void Update();

	// キー入力
	bool PushKey(BYTE keyNumber)const;
	bool TriggerKey(BYTE keyNumber) const;
	bool HoldKey(BYTE keyNumber) const;
	bool ReleaseKey(BYTE keyNumber) const;

	// マウス入力
	bool PushMouseButton(MouseButton mouseButton)const;
	bool TriggerMouseButton(MouseButton mouseButton)const;
	bool HoldMouseButton(MouseButton mouseButton)const;
	bool ReleaseMouseButton(MouseButton mouseButton)const;

	// ホイールの回転量を取得
	int64_t GetMouseWheelDelta() const;

private:
	// DirectInputの初期化
	void InitializeDirectInput();
	// キーボードの初期化
	void InitializeKeybord();
	// マウスの初期化
	void InitializeMouse();

	// キーボード入力の更新
	void UpdateKeybord();
	// マウス入力の更新
	void UpdateMouse();
private:
	// WindowAppをセット
	void SetWindowApp(WindowApp* windowApp);

private:
	// WindowAppのインスタンスを受け取る箱
	WindowApp* windowApp_ = nullptr;

	// DirectInput
	ComPtr <IDirectInput8> directInput_ = nullptr;
	// キーボード
	ComPtr <IDirectInputDevice8> keybord_ = nullptr;
	// マウス
	ComPtr <IDirectInputDevice8> mouse_ = nullptr;

	// 現在フレームのキー入力
	BYTE keys_[256];
	// 1フレーム前のキー入力
	BYTE preKeys_[256];

	// マウスの現在のボタン状態（DIMOUSESTATE で相対移動量 & ボタンの押下状態を保持）
	DIMOUSESTATE mouseState_{};
	// 1フレーム前の状態を保持する場合が必要なら
	DIMOUSESTATE prevMouseState_{};
};