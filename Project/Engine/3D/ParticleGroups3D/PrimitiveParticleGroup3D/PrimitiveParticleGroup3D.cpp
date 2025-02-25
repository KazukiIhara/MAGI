#include "PrimitiveParticleGroup3D.h"

#include "3D/Shapes3D/PrimitiveShapes3D/Plane/Plane.h"
#include "3D/Shapes3D/PrimitiveShapes3D/Sphere/Sphere.h"

#include "Framework/MAGI.h"

PrimitiveParticleGroup3D::PrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName)
	:BaseParticleGroup3D(particleGroupName) {
	// シンプル形状のタイプを設定
	primitiveType_ = primitiveType;

	// 貼り付けるテクスチャの名前を設定
	textureName_ = textureName;

}

void PrimitiveParticleGroup3D::AssignShape() {
	switch (primitiveType_.value()) {
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

void PrimitiveParticleGroup3D::Draw() {
	// パーティクルグループの描画前設定
	PrepareForRendering();

	// Texture用のSRVをセット
	uint32_t textureSrvIndex = MAGISYSTEM::GetTexture()[textureName_].srvIndex;
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(textureSrvIndex));

	// 描画
	primitive_->DrawInstanced(instanceCount_);
}
