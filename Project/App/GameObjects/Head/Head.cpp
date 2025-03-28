#include "Head.h"

#include "Framework/MAGI.h"

Head::Head(const std::string& objectName, const EulerTransform3D& transform)
	:GameObject3D(objectName, transform) {

}

void Head::Initialize() {
	SetParent(MAGISYSTEM::FindGameObject3D("Player"));
	
	GetTranslate().y = 1.0f;
}

void Head::Update() {
	if (MAGISYSTEM::TriggerKey(DIK_V)) {
		DetachParent();
	}
}
