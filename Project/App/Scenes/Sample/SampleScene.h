#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "2D/Object2D/Object2D.h"

// サンプルシーン
template <typename Data>
class SampleScene : public BaseScene<Data> {
public:
	using BaseScene<Data>::BaseScene; // 親クラスのコンストラクタをそのまま継承
	~SampleScene()override = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	// 地形
	std::unique_ptr<GameObject3D> terrain_ = nullptr;

	// オブジェクト
	std::unique_ptr<GameObject3D> sphere_[2] = nullptr;
};

template<typename Data>
inline void SampleScene<Data>::Initialize() {
	MAGISYSTEM::LoadModel("terrain", true);

	MAGISYSTEM::AddPunctualLight("sampleLight");
	auto& sampleLight = MAGISYSTEM::GetLightData("sampleLight");
	sampleLight.intensity = 1.0f;

	terrain_ = std::make_unique<GameObject3D>("Terrain");
	terrain_->CreateStaticRenderer("Terrain", "terrain");
}

template<typename Data>
inline void SampleScene<Data>::Update() {

	terrain_->Update();

}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	// 
	// オブジェクト3Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject3D();

	terrain_->Draw();

	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();


}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
