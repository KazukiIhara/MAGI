#include "BaseRenderable3D.h"
#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIMath;

BaseRenderable3D::BaseRenderable3D(const std::string& objectName) {
	Initialize(objectName);
}

void BaseRenderable3D::Update() {
	// ワールド行列更新
	worldTransform_.Update();
	// 中心座標の更新
	worldPosition_ = ExtractionWorldPos(worldTransform_.worldMatrix_);

	// uvTransformの更新
	material_.uvTransformMatrix = MakeUVMatrix(uvTransform_.scale, uvTransform_.rotateZ, uvTransform_.translate);

	// GPUに送るデータの更新
	UpdateWVPData();
	UpdateMaterialData();
}

Vector3& BaseRenderable3D::GetScale() {
	return worldTransform_.scale_;
}

Vector3& BaseRenderable3D::GetRotate() {
	return worldTransform_.rotate_;
}

Vector3& BaseRenderable3D::GetTranslate() {
	return worldTransform_.translate_;
}

UVTransform& BaseRenderable3D::GetUvTransform() {
	return uvTransform_;
}

Material3DForGPU& BaseRenderable3D::GetMaterial() {
	return material_;
}

void BaseRenderable3D::Initialize(const std::string& objectName) {
	// 名前のセット
	name_ = objectName;
	// ワールド行列初期化
	worldTransform_.Initialize();
	// UVトランスフォームを初期化
	uvTransform_ = {};

	// クオータニオン角を使用する
	worldTransform_.isUseQuaternion_ = false;
	// マテリアル初期化
	material_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_.enableLighting = true;
	material_.enableSpecularRef = false;
	material_.shininess = 100.0f;
	material_.uvTransformMatrix = MakeIdentityMatrix4x4();

	// WVP用のリソース作成
	CreateWVPResource();
	// データを書きこむ
	MapWVPData();
	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルデータ書き込み
	MapMateiralData();
}

void BaseRenderable3D::PrepareForRendering(bool isNormalMap) {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();

	// PSOを設定
	if (isNormalMap) {
		// NormalMapを使う
		commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3DNormalMap, blendMode_));
	} else {
		// NormalMapを使わない
		commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3D, blendMode_));
	}

	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
	// ライトを転送
	MAGISYSTEM::TransferPunctualLight();
	// カメラ情報を転送
	MAGISYSTEM::TransferCamera(2);
}

void BaseRenderable3D::CreateWVPResource() {
	// WVP用のリソースを作る
	transformationResource_ = MAGISYSTEM::CreateBufferResource(sizeof(TransformationMatrix));
}

void BaseRenderable3D::MapWVPData() {
	// データを書き込む
	transformationData_ = nullptr;
	// 書き込むためのアドレスを取得
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	// 単位行列を書き込んでおく
	transformationData_->World = MakeIdentityMatrix4x4();
	transformationData_->WorldInverseTransepose = MakeIdentityMatrix4x4();
}

void BaseRenderable3D::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Material3DForGPU));
}

void BaseRenderable3D::MapMateiralData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->enableSpecularRef = material_.enableSpecularRef;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}

void BaseRenderable3D::UpdateWVPData() {
	transformationData_->World = worldTransform_.worldMatrix_;
	transformationData_->WorldInverseTransepose = MakeInverseTransposeMatrix(worldTransform_.worldMatrix_);
}

void BaseRenderable3D::UpdateMaterialData() {
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->enableSpecularRef = material_.enableSpecularRef;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
	materialData_->shininess = material_.shininess;
}
