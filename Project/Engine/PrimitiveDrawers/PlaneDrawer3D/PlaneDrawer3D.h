#pragma once

// C++
#include <vector>

// DirectX
#include <d3d12.h>

#include "DirectX/ComPtr/ComPtr.h"
#include "Math/Utility/MathUtility.h"
#include "Structs/ObjectStruct.h"
#include "Structs/ColorStruct.h"
#include "Enums/BlendModeEnum.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

class DXGI;
class DirectXCommand;
class SRVUAVManager;
class GraphicsPipelineManager;
class Camera3DManager;

/// <summary>
/// 板ポリ描画クラス
/// </summary>
class PlaneDrawer3D {
public:
	PlaneDrawer3D(
		DXGI* dxgi,
		DirectXCommand* directXCommand,
		SRVUAVManager* srvUavManager,
		GraphicsPipelineManager* graphicsPipelineManager,
		Camera3DManager* camera3DManager
	);
	~PlaneDrawer3D();

	void Update();
	void Draw();

	void AddPlane(
		const Matrix4x4& worldMatrix,
		const Vector3& leftTop,
		const Vector3& rightTop,
		const Vector3& leftBottom,
		const Vector3& rightBottom,
		const RGBA& color,
		const uint32_t& textureIndex,
		const Vector2& uvScale,
		const float& uvRotate,
		const Vector2& uvTransform
	);

	void ClearPlanes();
	void SetDXGI(DXGI* dxgi);
	void SetDirectXCommand(DirectXCommand* directXCommand);
	void SetSRVUAVManager(SRVUAVManager* srvUavManager);
	void SetGraphicsPipelineManager(GraphicsPipelineManager* graphicsPipelineManager);
	void SetCamera3DManager(Camera3DManager* camera3DManager);
private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingデータ書き込み
	void MapInstancingData();

	// materialリソース作成
	void CreateMaterialResource();
	// materialデータ書き込み
	void MapMaterialData();

private:
	// 板ポリの最大数
	const uint32_t kNumMaxInstance = 32768;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

	// 板ポリデータ
	std::vector<PlaneData3DForGPU> planes_;
	// マテリアルデータ
	std::vector<PrimitiveMaterialData3DForGPU> materials_;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用のデータ
	PlaneData3DForGPU* instancingData_ = nullptr;

	// マテリアルのリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	PrimitiveMaterialData3DForGPU* materialData_ = nullptr;

	// Plane3DSrvIndex
	uint32_t planeSrvIndex = 0;
	// MaterialSrvIndex
	uint32_t materialSrvIndex_ = 0;

	// instance描画する際に使う変数
	uint32_t instanceCount_ = 0;

	// 現在のインデックス
	uint32_t currentIndex_ = 0;
private:
	DXGI* dxgi_ = nullptr;
	DirectXCommand* directXCommand_ = nullptr;
	SRVUAVManager* srvUavManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	Camera3DManager* camera3DManager_ = nullptr;
};