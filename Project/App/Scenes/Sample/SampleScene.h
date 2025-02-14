#pragma once

#include <memory>

#include "BaseScene/BaseScene.h"
#include "Framework/MAGI.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "2D/Object2D/Object2D.h"

// サンプルシーン
template <typename Data>
class SampleScene: public BaseScene<Data> {
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
	sampleLight.intensity = 0.5f;

	terrain_ = std::make_unique<GameObject3D>("Terrain");
	terrain_->CreateStaticRenderer("Terrain", "terrain");

	sphere_[0] = std::make_unique<GameObject3D>("Sphere0");
	sphere_[0]->CreatePrimitiveRenderer("Sphere0", Primitive3DType::Sphere);
	sphere_[0]->GetTranslate().y = 1.0f;
	sphere_[0]->GetTranslate().x = 1.5f;
	sphere_[0]->CreateCollider(Collider3DType::Sphere);

	sphere_[1] = std::make_unique<GameObject3D>("Sphere1");
	sphere_[1]->CreatePrimitiveRenderer("Sphere1", Primitive3DType::Sphere);
	sphere_[1]->GetTranslate().y = 1.0f;
	sphere_[1]->GetTranslate().x = -1.5f;
	sphere_[1]->CreateCollider(Collider3DType::Sphere);

}

template<typename Data>
inline void SampleScene<Data>::Update() {

	terrain_->Update();
	sphere_[0]->Update();
	sphere_[1]->Update();

}

template<typename Data>
inline void SampleScene<Data>::Draw() {
	// 
	// オブジェクト3Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject3D();

	terrain_->Draw();
	sphere_[0]->Draw();
	sphere_[1]->Draw();


	// 
	// オブジェクト2Dの描画前処理
	// 
	MAGISYSTEM::PreDrawObject2D();


}

template<typename Data>
inline void SampleScene<Data>::Finalize() {

}
