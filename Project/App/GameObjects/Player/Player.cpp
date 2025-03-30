#include "Player.h"

#include "Framework/MAGI.h"

Player::Player(const std::string& objectName, const EulerTransform3D& transform)
	:GameObject3D(objectName, transform) {

}

void Player::Initialize() {

}

void Player::Update() {
	if (MAGISYSTEM::PushKey(DIK_D)) {
		GetTranslate().x += 1.0f * MAGISYSTEM::GetDeltaTime();
	}
	if (MAGISYSTEM::PushKey(DIK_A)) {
		GetTranslate().x -= 1.0f * MAGISYSTEM::GetDeltaTime();
	}

	if (MAGISYSTEM::TriggerKey(DIK_G)) {
		DetachChild(GetChild("Head"));
	}

	if (MAGISYSTEM::TriggerButton(0, ButtonA)) {
		MAGISYSTEM::StartPadVibration(0, 1.0f, 1.0f, 1.0f);
	}

}
