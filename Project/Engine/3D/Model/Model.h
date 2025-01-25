#pragma once

// C++
#include <string>
#include <vector>

#include "3D/Mesh/Mesh.h"
#include "Structs/AnimationStruct.h"

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

	// 
	// forSkinning
	// 

	// スキニング用の情報
	SkinCluster skinCluster_;
	// スキニング用にシェーダーに渡す情報
	ComPtr<ID3D12Resource> skinningInformationResource_;
};