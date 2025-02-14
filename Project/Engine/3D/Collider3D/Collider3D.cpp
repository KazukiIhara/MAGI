#include "Collider3D.h"

#include "3D/GameObject3D/GameObject3D.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

Collider3D::Collider3D(GameObject3D* owner, Collider3DType type) {
	owner_ = owner;
	type_ = type;
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	// 親のゲームオブジェクトのトランスフォームを親子付け
	worldTransform_->parent_ = owner_->GetWorldTransform();
}

Collider3D::~Collider3D() {}

void Collider3D::Update() {
	// ワールド行列の更新
	worldTransform_->Update();
	worldPosition_ = ExtractionWorldPos(worldTransform_->worldMatrix_);
}

void Collider3D::Draw() {
	DrawSphere(worldPosition_, worldTransform_->scale_.x);
}

void Collider3D::DrawSphere(const Vector3& center, float radius) {
	uint32_t segments = 8;
	const int rings = segments;        // 緯度方向の分割数
	const int slices = segments * 2;  // 経度方向の分割数
	RGBA color = { 1.0f, 1.0f, 1.0f, 1.0f }; // デフォルトの色 (白)

	// π の定数を取得
	const float pi = std::numbers::pi_v<float>;

	// 経度方向の線を作成
	for (int ring = 0; ring <= rings; ++ring) {
		float theta = pi * ring / rings;
		float y = center.y + radius * cosf(theta);
		float r = radius * sinf(theta);

		for (int slice = 0; slice < slices; ++slice) {
			float phi1 = 2.0f * pi * slice / slices;
			float phi2 = 2.0f * pi * (slice + 1) / slices;

			Vector3 p1 = { center.x + r * cosf(phi1), y, center.z + r * sinf(phi1) };
			Vector3 p2 = { center.x + r * cosf(phi2), y, center.z + r * sinf(phi2) };

			// 線を描画
			MAGISYSTEM::DrawLine3D(p1, p2, color);
		}
	}

	// 緯度方向の線を作成
	for (int slice = 0; slice < slices; ++slice) {
		float phi = 2.0f * pi * slice / slices;

		for (int ring = 0; ring < rings; ++ring) {
			float theta1 = pi * ring / rings;
			float theta2 = pi * (ring + 1) / rings;

			Vector3 p1 = { center.x + radius * sinf(theta1) * cosf(phi), center.y + radius * cosf(theta1), center.z + radius * sinf(theta1) * sinf(phi) };
			Vector3 p2 = { center.x + radius * sinf(theta2) * cosf(phi), center.y + radius * cosf(theta2), center.z + radius * sinf(theta2) * sinf(phi) };

			// 線を描画
			MAGISYSTEM::DrawLine3D(p1, p2, color);
		}
	}
}

void Collider3D::DrawAABB() {

}

void Collider3D::DrawOBB() {

}
