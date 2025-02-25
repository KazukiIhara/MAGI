#pragma once

/// <summary>
/// パーティクルグループ
/// </summary>
class ParticleGroup3D {
public:
	ParticleGroup3D();
	~ParticleGroup3D();

	virtual void AssignShape() = 0;
	virtual void Update();
	virtual void Draw() = 0;


private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingリソース書き込み
	void MapInstancingData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();

};