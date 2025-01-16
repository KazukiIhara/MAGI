#include "TitleScene.h"

#include "Logger/Logger.h"

template<typename Data>
inline void TitleScene<Data>::Initialize() {
	Logger::Log("TitleScene Initialize\n");
}

template<typename Data>
void TitleScene<Data>::Update() {
	Logger::Log("TitleScene Update\n");
}

template<typename Data>
void TitleScene<Data>::Draw() {
	Logger::Log("TitleScene Draw\n");
}

template<typename Data>
void TitleScene<Data>::Finalize() {
	Logger::Log("TitleScene Finalize\n");
}


// 明示的インスタンシエーション
#include "GameData/GameData.h"
template class TitleScene<GameData>;
