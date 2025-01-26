#pragma once

// C++
#include <string>
#include <vector>
#include <memory>

#include "3D/Mesh/Mesh.h"
#include "3D/Skeleton/Skeleton.h"
#include "Structs/AnimationStruct.h"

class Model {
public:
	Model(const ModelData& modeldata);
	virtual ~Model();

	void Initialize(const ModelData& modeldata);
	virtual void Update();
	void Draw();

	bool IsNormalMap();

private:
	// 受け取ったモデルからメッシュを作成
	void CreateMehes();

protected:
	// 受け取るモデルデータ
	ModelData modelData_{};
	// メッシュ
	std::vector<Mesh> meshes_;
};