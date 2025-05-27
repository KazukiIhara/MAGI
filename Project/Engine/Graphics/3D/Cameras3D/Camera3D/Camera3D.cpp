#include "Camera3D.h"

#include <cassert>

#include "Framework/MAGI.h"

using namespace MAGIUtility;
using namespace MAGIMath;

Camera3D::Camera3D(const std::string& cameraName) {
	name = cameraName;
	Initialize();
}

Camera3D::~Camera3D() {

}

void Camera3D::Initialize() {
	// ワールド行列初期化
	transform_ = std::make_unique<Transform3D>(Vector3(1.0f, 1.0f, 1.0f), kDefaultCameraRotate_, kDefaultCameraTranslate_);
	transform_->Update();

	// ワールド座標を取得
	worldPosition = ExtractionWorldPos(transform_->GetWorldMatrix());

	// ビュー行列やらあれこれ
	viewMatrix_ = Inverse(transform_->GetWorldMatrix());
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, aspectRaito_, nearClipRange_, farClipRange_);
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	CreateCameraResource();
	MapCameraData();
}

void Camera3D::Update() {
	// カメラ特有の処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Rotate", &transform_->GetRotate().x, 0.01f);
	ImGui::End();



}

void Camera3D::UpdateData() {
	transform_->Update();

	Vector3 forward = {
	transform_->GetWorldMatrix().m[2][0],
	transform_->GetWorldMatrix().m[2][1],
	transform_->GetWorldMatrix().m[2][2]
	};

	ImGui::Text("Fwd: %.4f %.4f %.4f", forward.x, forward.y, forward.z);


	worldPosition = ExtractionWorldPos(transform_->GetWorldMatrix());
	viewMatrix_ = Inverse(transform_->GetWorldMatrix());
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	billboardMatrix_ = transform_->GetWorldMatrix();
	// 平行移動成分を削除
	billboardMatrix_.m[3][0] = 0.0f;
	billboardMatrix_.m[3][1] = 0.0f;
	billboardMatrix_.m[3][2] = 0.0f;

	const Matrix4x4& m = viewProjectionMatrix_;

	// 各平面の抽出（行ベクトル形式）
	frustumPlanes_[0] = NormalizePlane(Vector4(
		m.m[0][3] + m.m[0][0],
		m.m[1][3] + m.m[1][0],
		m.m[2][3] + m.m[2][0],
		m.m[3][3] + m.m[3][0])); // Left

	frustumPlanes_[1] = NormalizePlane(Vector4(
		m.m[0][3] - m.m[0][0],
		m.m[1][3] - m.m[1][0],
		m.m[2][3] - m.m[2][0],
		m.m[3][3] - m.m[3][0])); // Right

	frustumPlanes_[2] = NormalizePlane(Vector4(
		m.m[0][3] + m.m[0][1],
		m.m[1][3] + m.m[1][1],
		m.m[2][3] + m.m[2][1],
		m.m[3][3] + m.m[3][1])); // Bottom

	frustumPlanes_[3] = NormalizePlane(Vector4(
		m.m[0][3] - m.m[0][1],
		m.m[1][3] - m.m[1][1],
		m.m[2][3] - m.m[2][1],
		m.m[3][3] - m.m[3][1])); // Top

	frustumPlanes_[4] = NormalizePlane(Vector4(
		m.m[0][2],
		m.m[1][2],
		m.m[2][2],
		m.m[3][2])); // Near

	frustumPlanes_[5] = NormalizePlane(Vector4(
		m.m[0][3] - m.m[0][2],
		m.m[1][3] - m.m[1][2],
		m.m[2][3] - m.m[2][2],
		m.m[3][3] - m.m[3][2])); // Far

	UpdateCameraData();
}

void Camera3D::DrawFrustum() {
    // NDC空間の8頂点（Z: [0=Near, 1=Far]、左手系）
    static const Vector3 ndcCorners[8] =
    {
        { -1,  1, 0 }, // 0: Near-Top-Left
        {  1,  1, 0 }, // 1: Near-Top-Right
        { -1, -1, 0 }, // 2: Near-Bottom-Left
        {  1, -1, 0 }, // 3: Near-Bottom-Right
        { -1,  1, 1 }, // 4: Far-Top-Left
        {  1,  1, 1 }, // 5: Far-Top-Right
        { -1, -1, 1 }, // 6: Far-Bottom-Left
        {  1, -1, 1 }, // 7: Far-Bottom-Right
    };

    // ビュー・プロジェクション逆行列を個別に取得（安定性重視）
    const Matrix4x4 invView = Inverse(viewMatrix_);
    const Matrix4x4 invProj = Inverse(projectionMatrix_);

    // ワールド空間のFrustum頂点
    Vector3 worldCorners[8];
    for (int i = 0; i < 8; ++i) {
        Vector3 ndc = ndcCorners[i];

        // Clip空間座標
        Vector4 clip = Vector4(ndc.x, ndc.y, ndc.z, 1.0f);

        // View空間へ
        Vector4 view = Transform(clip, invProj);
        if (abs(view.w) > 1e-5f) {
            view /= view.w;
        }

        // ワールド空間へ
        Vector4 world = Transform(view, invView);
        worldCorners[i] = Vector3(world.x, world.y, world.z) / world.w;
    }

    // 色指定
    const Vector4 color = Color::Yellow;

    // Near平面
    MAGISYSTEM::DrawLine3D(worldCorners[0], worldCorners[1], color);
    MAGISYSTEM::DrawLine3D(worldCorners[1], worldCorners[3], color);
    MAGISYSTEM::DrawLine3D(worldCorners[3], worldCorners[2], color);
    MAGISYSTEM::DrawLine3D(worldCorners[2], worldCorners[0], color);

    // Far平面
    MAGISYSTEM::DrawLine3D(worldCorners[4], worldCorners[5], color);
    MAGISYSTEM::DrawLine3D(worldCorners[5], worldCorners[7], color);
    MAGISYSTEM::DrawLine3D(worldCorners[7], worldCorners[6], color);
    MAGISYSTEM::DrawLine3D(worldCorners[6], worldCorners[4], color);

    // Near↔Farの辺
    MAGISYSTEM::DrawLine3D(worldCorners[0], worldCorners[4], color);
    MAGISYSTEM::DrawLine3D(worldCorners[1], worldCorners[5], color);
    MAGISYSTEM::DrawLine3D(worldCorners[2], worldCorners[6], color);
    MAGISYSTEM::DrawLine3D(worldCorners[3], worldCorners[7], color);
}

void Camera3D::TransferCamera(uint32_t rootParameterIndex) {
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, cameraResource_->GetGPUVirtualAddress());
}

void Camera3D::TransferCameraInv(uint32_t rootParameterIndex) {
	MAGISYSTEM::GetDirectXCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex, cameraInvResource_->GetGPUVirtualAddress());
}

Vector3& Camera3D::GetRotate() {
	return transform_->GetRotate();
}

Vector3& Camera3D::GetTranslate() {
	return transform_->GetTranslate();
}

Matrix4x4 Camera3D::GetViewProjectionMatrix() const {
	return viewProjectionMatrix_;
}

float Camera3D::GetFarClipRange() const {
	return farClipRange_;
}

void Camera3D::CreateCameraResource() {
	cameraResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Camera3DForGPU));
	cameraInvResource_ = MAGISYSTEM::CreateBufferResource(sizeof(Camera3DInverseForGPU));
}

void Camera3D::MapCameraData() {
	cameraData_ = nullptr;
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));

	cameraInvData_ = nullptr;
	cameraInvResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraInvData_));
	UpdateCameraData();
}

void Camera3D::UpdateCameraData() {
	cameraData_->worldPosition = worldPosition;
	cameraData_->viewProjection = viewProjectionMatrix_;

	cameraInvData_->invView = Inverse(viewMatrix_);
	cameraInvData_->invProj = Inverse(projectionMatrix_);
}
