#include "SampleScene.h"

#include <iostream>

#include "Framework/MAGI.h"
#include "Logger/Logger.h"

template <typename Data>
void SampleScene<Data>::Initialize() {
	Logger::Log("SampleScene Initialize\n");
}

template <typename Data>
void SampleScene<Data>::Update() {
	Logger::Log("SampleScene Update\n");

	if (MAGISYSTEM::TriggerKey(DIK_SPACE)) {
		this->sceneManager_->ChangeScene("Title");
	}
}

template <typename Data>
void SampleScene<Data>::Draw() {
	Logger::Log("SampleScene Draw\n");
}

template <typename Data>
void SampleScene<Data>::Finalize() {
	Logger::Log("SampleScene Finalize\n");
}

// 明示的インスタンシエーション
#include "GameData/GameData.h"
template class SampleScene<GameData>;

