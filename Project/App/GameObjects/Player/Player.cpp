#include "Player.h"

#include "Framework/MAGI.h"

Player::Player(const std::string& objectName, const EulerTransform3D& transform)
	:GameObject3D(objectName, transform) {

}

void Player::Initialize() {

}

void Player::Update() {
	if (MAGISYSTEM::PushKey(DIK_D)) {
		GetTranslate().x += 0.01f;
	}
	if (MAGISYSTEM::PushKey(DIK_A)) {
		GetTranslate().x -= 0.01f;
	}
}
