#include "PrimitiveRenderer3D.h"

#include "3D/Shapes3D/PrimitiveShapes3D/Plane/Plane.h"
#include "3D/Shapes3D/PrimitiveShapes3D/Sphere/Sphere.h"

#include "Framework/MAGI.h"

PrimitiveRenderer3D::PrimitiveRenderer3D(const std::string& objectName, const Primitive3DType& primitiveType, const std::string& textureName)
	:BaseRenderable3D(objectName) {
	primitiveType_ = primitiveType;
	textureName_ = textureName;
	// テクスチャが未設定の場合、デフォルトのテクスチャを割り当てる(uvCheckerはエンジン用のリソースのためフルパス指定)
	if (textureName_ == "") {
		textureName_ = "Resources/Images/uvChecker.png";
	}
}

PrimitiveRenderer3D::~PrimitiveRenderer3D() {
	primitive_.reset();
}

void PrimitiveRenderer3D::Update() {
	// Primitive更新
	primitive_->Update();
	// 基底クラスの更新
	BaseRenderable3D::Update();
}

void PrimitiveRenderer3D::Draw() {
	// 描画の設定
	PrepareForRendering();

	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[textureName_].srvIndex;
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvDescriptorHandleGPU(textureSrvIndex));

	// NormalMap用のSrvをセット
	if (normalMapTextureName_ != "") {
		uint32_t normalMapTextureSrvIndex = MAGISYSTEM::GetTexture()[normalMapTextureName_].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvDescriptorHandleGPU(normalMapTextureSrvIndex));
	} else {// 未定義動作を防ぐため、デフォルトのテクスチャのsrvIndexをセットしておく
		uint32_t defaultNormalMapTextureSrvIndex = MAGISYSTEM::GetTexture()["Resources/Images/uvChecker.png"].srvIndex;
		commandList->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvDescriptorHandleGPU(defaultNormalMapTextureSrvIndex));
	}

	// Primitive描画
	primitive_->Draw();
}

void PrimitiveRenderer3D::AssignShape() {
	switch (primitiveType_) {
		case Primitive3DType::Plane:
			primitive_ = std::make_unique<Plane>();
			break;
		case Primitive3DType::Sphere:
			primitive_ = std::make_unique<Sphere>();
			break;
		case Primitive3DType::Ring:
			break;
		case Primitive3DType::Cylinder:
			break;
	}
}