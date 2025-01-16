#pragma once

#include "BaseScene/BaseScene.h"

// サンプルシーン
template <typename Data>
class SampleScene2: public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

};

template<typename Data>
inline void SampleScene2<Data>::Initialize() {

}

template<typename Data>
inline void SampleScene2<Data>::Update() {
	Logger::Log("SampleScene2 Update");
	this->GetData().score += 1;
	int score = this->GetData().score;
	std::string string = std::to_string(score);
	Logger::Log(string + "\n");

}

template<typename Data>
inline void SampleScene2<Data>::Draw() {

}

template<typename Data>
inline void SampleScene2<Data>::Finalize() {

}
