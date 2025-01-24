#pragma once

// C++
#include <string>
#include <vector>

#include "3D/Mesh/Mesh.h"

class Model {
public:
	Model(const ModelData& modeldata);
	~Model();

	void Initialize(const ModelData& modeldata);
	void Update();
	void Draw();

	bool IsNormalMap();
private:
	// 受け取ったモデルからメッシュを作成
	void CreateMehes();
private:
	// 受け取るモデルデータ
	ModelData modelData_{};
	// メッシュ
	std::vector<Mesh> meshes_;
};