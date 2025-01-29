#include "Object2D.h"

#include "Framework/MAGI.h"

#include "DirectXTex/DirectXTex.h"

using namespace MAGIMath;

Object2D::Object2D(const std::string& objectName, const std::string& textureName) {

}

Object2D::~Object2D() {

}

void Object2D::Initialize(const std::string& objectName, const std::string& textureName) {
	// 名前をセット
	name_ = objectName;
	// トランスフォームを初期化
	worldTransform_.Update();
	// マテリアルを初期化
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.uvTransformMatrix = MakeIdentityMatrix4x4();
	// テクスチャ左上座標
	textureLeftTop_ = { 0.0f,0.0f };

	// テクスチャをロード
	MAGISYSTEM::LoadTexture(textureName);

	// 頂点リソースの作成
	CreateVertexResource();
	// 頂点バッファビューの作成
	CreateVretexBufferView();
	// 頂点データの書き込み
	MapVertexData();

	// 描画用のインデックスリソースを作成
	CreateIndexResource();
	// インデックスバッファビューの作成
	CreateIndexBufferView();
	// インデックスリソースにデータを書き込む
	MapIndexResource();

	// マテリアル用のリソース作成
	CreateMaterialResource();
	// マテリアルにデータを書き込む
	MapMaterialData();

	// wvp用のリソース作成
	CreateWVPResource();
	// データを書き込む
	MapWVPData();

	// テクスチャサイズを設定
	AdjustTextureSize();

}

void Object2D::Update() {
	// アンカーポイントの設定
	float left = 0.0f - anchorPoint_.x;
	float right = 01.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// データを書き込む
	vertexData_[0].position = { left,bottom,0.0f,1.0f };//左下
	vertexData_[1].position = { left,top,0.0f,1.0f };//左上
	vertexData_[2].position = { right,bottom,0.0f,1.0f };//右下
	vertexData_[3].position = { right,top,0.0f,1.0f };//右上

	// メタデータを取得
	const DirectX::TexMetadata& metaData =
		MAGISYSTEM::GetTextureMetaData(textureName_);
	float texLeft = textureLeftTop_.x / metaData.width;
	float texRight = (textureLeftTop_.x + cutOutSize_.x) / metaData.width;
	float texTop = textureLeftTop_.y / metaData.height;
	float texBottom = (textureLeftTop_.y + cutOutSize_.y) / metaData.height;

	// データを書き込む
	vertexData_[0].texcoord = { texLeft,texBottom };
	vertexData_[1].texcoord = { texLeft,texTop };
	vertexData_[2].texcoord = { texRight,texBottom };
	vertexData_[3].texcoord = { texRight,texTop };

	// ワールド行列の更新
	worldTransform_.Update();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(GetSystemMetrics(SM_CXSCREEN)), float(GetSystemMetrics(SM_CYSCREEN)), 0.0f, 100.0f);
	*wvpData_ = worldTransform_.worldMatrix_ * projectionMatrix;

	// マテリアルデータの更新
	material_.uvTransformMatrix = MakeUVMatrix(uvTransform_.scale, uvTransform_.rotateZ, uvTransform_.translate);
	materialData_->uvTransformMatrix = material_.uvTransformMatrix;

}

void Object2D::Draw() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object2D, blendMode_));
	// VBVの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	// IBVの設定
	commandList->IASetIndexBuffer(&indexBufferView_);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());
	// テクスチャのSRVを設定
	uint32_t textureSrv = MAGISYSTEM::GetTexture()[textureName_].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvDescriptorHandleGPU(textureSrv));
	// 描画！(DrawCall/ドローコール)
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Object2D::CreateVertexResource() {

}

void Object2D::CreateVretexBufferView() {

}

void Object2D::MapVertexData() {

}

void Object2D::CreateIndexResource() {

}

void Object2D::CreateIndexBufferView() {

}

void Object2D::MapIndexResource() {

}

void Object2D::CreateMaterialResource() {

}

void Object2D::MapMaterialData() {

}

void Object2D::CreateWVPResource() {

}

void Object2D::MapWVPData() {

}

void Object2D::AdjustTextureSize() {

}
