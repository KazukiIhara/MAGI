#include "SphereCollider.h"

#include "Framework/MAGI.h"

SphereCollider::SphereCollider(uint32_t id, Collider3DType type)
	:BaseCollider3D(id, type) {

}

void SphereCollider::Draw() {
	uint32_t segments = 8;
	const int rings = segments;        // 緯度方向の分割数
	const int slices = segments * 2;  // 経度方向の分割数
	RGBA color = { 1.0f, 1.0f, 1.0f, 1.0f }; // デフォルトの色 (白)

	// π の定数を取得
	const float pi = std::numbers::pi_v<float>;

	// 経度方向の線を作成
	for (int ring = 0; ring <= rings; ++ring) {
		float theta = pi * ring / rings;
		float y = worldPosition_.y + radius_ * cosf(theta);
		float r = radius_ * sinf(theta);

		for (int slice = 0; slice < slices; ++slice) {
			float phi1 = 2.0f * pi * slice / slices;
			float phi2 = 2.0f * pi * (slice + 1) / slices;

			Vector3 p1 = { worldPosition_.x + r * cosf(phi1), y, worldPosition_.z + r * sinf(phi1) };
			Vector3 p2 = { worldPosition_.x + r * cosf(phi2), y, worldPosition_.z + r * sinf(phi2) };

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

			Vector3 p1 = { worldPosition_.x + radius_ * sinf(theta1) * cosf(phi), worldPosition_.y + radius_ * cosf(theta1), worldPosition_.z + radius_ * sinf(theta1) * sinf(phi) };
			Vector3 p2 = { worldPosition_.x + radius_ * sinf(theta2) * cosf(phi), worldPosition_.y + radius_ * cosf(theta2), worldPosition_.z + radius_ * sinf(theta2) * sinf(phi) };

			// 線を描画
			MAGISYSTEM::DrawLine3D(p1, p2, color);
		}
	}


}
