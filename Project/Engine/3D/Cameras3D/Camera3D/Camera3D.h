#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"
#include "Structs/CameraStruct.h"

/// <summary>
/// 3D用カメラ
/// </summary>
class Camera3D:public WorldEntity {
public:
	Camera3D(const std::string& cameraName);
	virtual ~Camera3D()override;

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void UpdateData();
	// 定数バッファに転送
	void TransferCamera(uint32_t rootParameterIndex);


	// ビュープロジェクションマトリックスを送る
	Matrix4x4 GetViewProjectionMatrix()const;

private:
	// カメラのリソースを作成
	void CreateCameraResource();
	// カメラのデータを入力
	void MapCameraData();
	// カメラのデータを更新
	void UpdateCameraData();

protected:
	// カメラの初期トランスフォーム
	const Vector3 kDefaultCameraRotate_ = { 0.45f,0.0f,0.0f };
	const Vector3 kDefaultCameraTranslate_ = { 0.0f,2.0f,-3.0f };
	// ワールドトランスフォーム
	WorldTransform worldTransform_{};
	// プロジェクション行列
	Matrix4x4 projectionMatrix_{};
	// ビュープロジェクションマトリックス
	Matrix4x4 viewProjectionMatrix_{};
	// 水平視野角(度数法)
	float fovYDegrees_ = 60.0f;
	// 水平方向視野角
	float fovY_ = fovYDegrees_ * (std::numbers::pi_v<float> / 180.0f);
	// アスペクト比
	float aspectRaito_ = 16.0f / 9.0f;
	// ニアクリップ距離
	float nearClipRange_ = 0.1f;
	// ファークリップ距離
	float farClipRange_ = 1000.0f;
	// ビルボード行列
	Matrix4x4 billboardMatrix_{};
	// 有効フラグ
	bool isActive_ = true;
private:
	// Camera用リソース
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	// Camera用データ
	CameraForGPU* cameraData_ = nullptr;
};