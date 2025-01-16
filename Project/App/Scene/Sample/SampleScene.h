#pragma once

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

// サンプルシーン
template <typename Data>
class SampleScene: public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:

};

template<typename Data>
inline void SampleScene<Data>::Initialize() {

}

template<typename Data>
inline void SampleScene<Data>::Update() {

}

template<typename Data>
inline void SampleScene<Data>::Draw() {

}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
