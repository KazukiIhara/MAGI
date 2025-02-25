#pragma once

// C++
#include <cstdint>
#include <list>
#include <optional>
#include <string>

#include "Structs/ParticleStruct.h"
#include "Enums/Renderer3DEnum.h"
#include "Enums/BlendModeEnum.h"

#include "DirectX/ComPtr/ComPtr.h"

#include <d3d12.h>

/// <summary>
/// パーティクルグループ
/// </summary>
class BaseParticleGroup3D {
public:
	BaseParticleGroup3D();
	~BaseParticleGroup3D();

	virtual void AssignShape() = 0;
	virtual void Update();
	virtual void Draw() = 0;

	// 新規パーティクルの追加
	void AddNewParticle(const EmitParamater& emitSetting);

private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingリソース書き込み
	void MapInstancingData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();

public:
	// パーティクルグループの名前
	std::string name = "";
private:
	// パーティクルのリスト
	std::list<ParticleData> particles_;
	// パーティクルの最大数
	const uint32_t kNumMaxInstance_ = 1024;
	// 描画タイプ
	std::optional<Renderer3DType> rendererType_ = std::nullopt;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Add;
	// instance描画する際に使う変数
	uint32_t instanceCount_ = 0;
private:
	// instancing描画用リソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用データ
	ParticleForGPU* instancingData_ = nullptr;
	// SrvIndex
	uint32_t srvIndex_ = 0;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material3DForGPU* materialData_ = nullptr;
	// マテリアル
	Material3D material_{};
};