#pragma once

#include <XInput.h>
#include "Enums/XInputEnum.h"

/// <summary>
/// Xインプットクラス
/// </summary>
class MAGIXInput {
public:
	MAGIXInput();
	~MAGIXInput();

	// 更新
	void Update();

	// パッドの接続状況を確認
	bool IsConnected(int controllerID) const;

	// コントローラのボタン入力
	bool PushButton(int controllerID, int buttonNumber) const;
	bool TriggerButton(int controllerID, int buttonNumber) const;
	bool HoldButton(int controllerID, int buttonNumber) const;
	bool ReleaseButton(int controllerID, int buttonNumber) const;

	// スティックやトリガーの位置取得
	int GetLeftStickX(int controllerID) const;
	int GetLeftStickY(int controllerID) const;
	int GetRightStickX(int controllerID) const;
	int GetRightStickY(int controllerID) const;
	int GetLeftTrigger(int controllerID) const;
	int GetRightTrigger(int controllerID) const;

	// 方向キー
	bool IsPadUp(int controllerID) const;
	bool IsPadRight(int controllerID) const;
	bool IsPadDown(int controllerID) const;
	bool IsPadLeft(int controllerID) const;

	// デッドゾーンの設定
	void SetDeadZone(int deadZone);
	// デッドゾーンの取得
	int GetDeadZone() const;

private:
	int ProcessDeadZone(int value) const;
private:
	// 最大4つのコントローラの状態を管理
	XINPUT_STATE gamepadStates[4]{};
	XINPUT_STATE preGamepadStates[4]{};
	// スティックの遊び
	int deadZone_ = 25;
};