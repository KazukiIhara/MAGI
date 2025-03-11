#include "MAGIXInput.h"

#include <cassert>
#include "math.h"
#include "math.h"

MAGIXInput::MAGIXInput() {}

MAGIXInput::~MAGIXInput() {}

void MAGIXInput::Update() {
	for (int i = 0; i < 4; ++i) {
		preGamepadStates[i] = gamepadStates[i];
		DWORD dwResult = XInputGetState(i, &gamepadStates[i]);
		if (dwResult != ERROR_SUCCESS) {
			ZeroMemory(&gamepadStates[i], sizeof(XINPUT_STATE));
		}
	}
}

bool MAGIXInput::IsConnected(int controllerID) const {
	if (controllerID < 0 || controllerID >= 4) {
		return false; // 範囲外のIDなら接続されていないと判断
	}
	XINPUT_STATE state;
	DWORD dwResult = XInputGetState(controllerID, &state);
	return (dwResult == ERROR_SUCCESS);
}

bool MAGIXInput::PushButton(int controllerID, int buttonNumber) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & buttonNumber) != 0;
}

bool MAGIXInput::TriggerButton(int controllerID, int buttonNumber) const {
	return !(preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool MAGIXInput::HoldButton(int controllerID, int buttonNumber) const {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool MAGIXInput::ReleaseButton(int controllerID, int buttonNumber) const {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		!(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

int MAGIXInput::GetLeftStickX(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbLX);
}

int MAGIXInput::GetLeftStickY(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbLY);
}

int MAGIXInput::GetRightStickX(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbRX);
}

int MAGIXInput::GetRightStickY(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbRY);
}

int MAGIXInput::GetLeftTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bLeftTrigger;
}

int MAGIXInput::GetRightTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bRightTrigger;
}

bool MAGIXInput::IsPadUp(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
}

bool MAGIXInput::IsPadRight(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
}

bool MAGIXInput::IsPadDown(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
}

bool MAGIXInput::IsPadLeft(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
}

void MAGIXInput::SetDeadZone(int deadZone) {
	deadZone_ = (deadZone < 0) ? 0 : (deadZone > 100) ? 100 : deadZone;
}

int MAGIXInput::GetDeadZone() const {
	return deadZone_;
}

int MAGIXInput::ProcessDeadZone(int value) const {
	int maxValue = 32767; // XInputのスティックの最大値
	int threshold = maxValue * deadZone_ / 100;

	if (abs(value) < threshold) {
		return 0;
	} else {
		return (value > 0) ? (value - threshold) * maxValue / (maxValue - threshold)
			: (value + threshold) * maxValue / (maxValue - threshold);
	}
}
