#include "DirectInput.h"

#include <cassert>

#include "Logger/Logger.h"
#include "WindowApp/WindowApp.h"

DirectInput::DirectInput(WindowApp* windowApp) {
	Initialize(windowApp);
	Logger::Log("DirectInput Initialize\n");
}

DirectInput::~DirectInput() {
	Logger::Log("DirectInput Finalize\n");
}

void DirectInput::Initialize(WindowApp* windowApp) {
	SetWindowApp(windowApp);
	InitializeDirectInput();
	InitializeKeybord();
	InitializeMouse();
}

void DirectInput::Update() {
	// キーボード入力を更新
	UpdateKeybord();
	// マウス入力の更新
	UpdateMouse();
}

bool DirectInput::PushKey(BYTE keyNumber) const {
	return keys_[keyNumber] != 0;
}

bool DirectInput::TriggerKey(BYTE keyNumber) const {
	return !preKeys_[keyNumber] && keys_[keyNumber];
}

bool DirectInput::HoldKey(BYTE keyNumber) const {
	return preKeys_[keyNumber] && keys_[keyNumber];
}

bool DirectInput::ReleaseKey(BYTE keyNumber) const {
	return preKeys_[keyNumber] && !keys_[keyNumber];
}

bool DirectInput::PushMouseButton(MouseButton mouseButton) const {
	// 現在のフレームでマウスボタンが押されているかを判定
	return (mouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) != 0;
}

bool DirectInput::TriggerMouseButton(MouseButton mouseButton) const {
	// 前フレームでは押されておらず、現在フレームで押されたかを判定
	return !(prevMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) &&
		(mouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80);
}

bool DirectInput::HoldMouseButton(MouseButton mouseButton) const {
	// 前フレームでも現在フレームでも押され続けているかを判定
	return (prevMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) &&
		(mouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80);
}

bool DirectInput::ReleaseMouseButton(MouseButton mouseButton) const {
	// 前フレームでは押されており、現在フレームで離されたかを判定
	return (prevMouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80) &&
		!(mouseState_.rgbButtons[static_cast<int>(mouseButton)] & 0x80);
}

int64_t DirectInput::GetMouseWheelDelta() const {
	// マウスのホイール回転量を取得（DIMOUSESTATEのz軸移動量がホイール回転に対応）
	return static_cast<int64_t>(mouseState_.lZ);
}

void DirectInput::InitializeDirectInput() {
	// DirectInput本体の初期化
	HRESULT result = S_FALSE;
	directInput_ = nullptr;
	result = DirectInput8Create(
		windowApp_->GetWndClass().hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput_,
		nullptr
	);
	assert(SUCCEEDED(result));
}

void DirectInput::InitializeKeybord() {
	// キーボードの初期化
	HRESULT result = S_FALSE;
	keybord_ = nullptr;
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keybord_, NULL);
	assert(SUCCEEDED(result));

	result = keybord_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	result = keybord_->SetCooperativeLevel(windowApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// キーの状態をリセット
	memset(keys_, 0, sizeof(keys_));
	memset(preKeys_, 0, sizeof(preKeys_));
}

void DirectInput::InitializeMouse() {
	// マウスの初期化
	HRESULT result = S_FALSE;
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));

	// マウスのデータ形式 (相対座標 + ボタン状態など)
	// c_dfDIMouse: ボタンは4つまで, c_dfDIMouse2だと8つまで扱える
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	// キーボードと同様に協調レベルを設定
	result = mouse_->SetCooperativeLevel(
		windowApp_->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	assert(SUCCEEDED(result));
}

void DirectInput::UpdateKeybord() {
	// デバイスを取得 (Acquire)
	keybord_->Acquire();
	// 前フレームの状態を保存
	memcpy(preKeys_, keys_, sizeof(keys_));
	// 現在のキーボード状態を取得
	keybord_->GetDeviceState(sizeof(keys_), keys_);
}

void DirectInput::UpdateMouse() {
	// デバイスを取得 (Acquire)
	mouse_->Acquire();
	// 前フレームのマウス状態を保存（押し始め/離し始めを判定したい場合に使う）
	prevMouseState_ = mouseState_;

	// 今フレームの状態取得
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState_);
}

void DirectInput::SetWindowApp(WindowApp* windowApp) {
	assert(windowApp);
	windowApp_ = windowApp;
}
