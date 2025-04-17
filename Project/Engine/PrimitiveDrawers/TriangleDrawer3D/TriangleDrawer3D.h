#pragma once

#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ObjectStruct.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

class TriangleDrawer3D {
public:
	TriangleDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~TriangleDrawer3D();

	void Update();
	void Draw();

	void AddTriangle(
		const Matrix4x4& worldMatrix,
		const PrimitiveMaterialData3D& material
		);

private:
	void ClearTriangles();
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingデータ書き込み
	void MapInstancingData();

	// materialリソース作成
	void CreateMaterialResource();
	// materialデータ書き込み
	void MapMaterialData();

	// 三角形の最大数
	const uint32_t kNumMaxInstance = 65535;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

	// 三角形データコンテナ
	std::vector<TriangleData3DForGPU> triangles_;
	// マテリアルデータコンテナ
	std::vector<PrimitiveMaterialData3DForGPU> materials_;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	TriangleData3DForGPU* instancingData_ = nullptr;

	// マテリアルのリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	PrimitiveMaterialData3DForGPU* materialData_ = nullptr;

	// Triangle3DSrvIndex
	uint32_t instancingSrvIndex_ = 0;
	// MaterialSrvIndex
	uint32_t materialSrvIndex_ = 0;

	// instance描画する際に使う変数
	uint32_t instanceCount_ = 0;

	// 三角形追加時のインデックス
	uint32_t currentIndex_ = 0;

private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;

};