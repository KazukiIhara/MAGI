#include "Object3D.h"

#include <cassert>

#include "Framework/MAGI.h"

#include "3D/Camera3D/Camera3D.h"

using namespace MAGIMath;

Object3D::Object3D(const std::string& modelName) {
	Initialize(modelName);
}

Object3D::~Object3D() {
	model_.reset();
}

void Object3D::Initialize(const std::string& modelName) {
	// ワールド行列初期化
	worldTransform_.Initialize();

	// モデル取得
	model_ = std::make_unique<Model>(MAGISYSTEM::FindModel(modelName));
	assert(model_);

	// マテリアル初期化
	material_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_.enableLighting = true;
	material_.shininess = 40.0f;
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

void Object3D::Update() {
	// モデル更新
	model_->Update();
	// ワールド行列更新
	worldTransform_.Update();

	UpdateWVPData();

	UpdateMaterialData();

}

void Object3D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3D, blendMode_));
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
	// ライトを転送
	MAGISYSTEM::TransferPunctualLight();

	// カメラ情報を転送
	camera3d_->TransferCamera();
	// 3Dモデル描画
	model_->Draw();
}

void Object3D::CreateWVPResource() {
	// WVP用のリソースを作る
	transformationResource_ = MAGISYSTEM::CreateBufferResource(sizeof(TransformationMatrix));
}

void Object3D::MapWVPData() {
	// データを書き込む
	transformationData_ = nullptr;
	// 書き込むためのアドレスを取得
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	// 単位行列を書き込んでおく
	transformationData_->World = MakeIdentityMatrix4x4();
	transformationData_->ViewProjection = MakeIdentityMatrix4x4();
	transformationData_->WorldInverseTransepose = MakeIdentityMatrix4x4();
}

void Object3D::CreateMaterialResource() {
	// マテリアル用のリソース作成
	materialResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Material3D));
}

void Object3D::MapMateiralData() {
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}

void Object3D::UpdateWVPData() {
	transformationData_->World = worldTransform_.worldMatrix_;
	transformationData_->ViewProjection = camera3d_->GetViewProjectionMatrix();
	transformationData_->WorldInverseTransepose = MakeInverseTransposeMatrix(worldTransform_.worldMatrix_);
}

void Object3D::UpdateMaterialData() {
	materialData_->color = material_.color;
	materialData_->enableLighting = material_.enableLighting;
	materialData_->shininess = material_.shininess;
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;
}
