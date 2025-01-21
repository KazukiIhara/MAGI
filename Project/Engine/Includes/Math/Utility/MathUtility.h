#pragma once

// C++
#include <cmath>
#include <algorithm>
#include <numbers>
#include <vector>

// MyHedder
#include "Math/Types/Vector2.h"
#include "Math/Types/Vector3.h"
#include "Math/Types/Vector4.h"

#include "Math/Types/Matrix2x2.h"
#include "Math/Types/Matrix3x3.h"
#include "Math/Types/Matrix4x4.h"

#include "Math/Types/Quaternion.h"

///
/// Vector2
///

Vector2 operator-(const Vector2& v);
Vector2 operator+(const Vector2& v);

///
/// Vector2 & Vector2
///

Vector2 operator+(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Vector2& v1, const Vector2& v2);
Vector2 operator/(const Vector2& v1, const Vector2& v2);

///
/// Vector2 & Scaler
///

Vector2 operator/(const Vector2& v, float s);
Vector2 operator*(float s, const Vector2& v);
Vector2 operator*(const Vector2& v, float s);

///
/// Vector3
///

Vector3 operator-(const Vector3& v);
Vector3 operator+(const Vector3& v);

///
/// Vector3 & Vector3
///

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(const Vector3& v1, const Vector3& v2);
Vector3 operator/(const Vector3& v1, const Vector3& v2);

///
/// Vector3 & Scaler
///

Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);
Vector3 operator/(const Vector3& v, float s);

///
/// Vector4
///

Vector4 operator-(const Vector4& v);
Vector4 operator+(const Vector4& v);

///
/// Vector4 & Vector4
///

Vector4 operator+(const Vector4& v1, const Vector4& v2);
Vector4 operator-(const Vector4& v1, const Vector4& v2);
Vector4 operator*(const Vector4& v1, const Vector4& v2);
Vector4 operator/(const Vector4& v1, const Vector4& v2);

///
/// Vector4 & Scaler
///

Vector4 operator*(float s, const Vector4& v);
Vector4 operator*(const Vector4& v, float s);
Vector4 operator/(const Vector4& v, float s);

///
/// Matrix4x4
///

///
/// Matrix4x4 & Matrix4x4
///

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

///
/// Quaternion
///

///
/// Quaternion & Quaternion
///

Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);



/// <summary>
/// 数学関数群　(左手座標系、行ベクトル)
/// </summary>
namespace MAGIMath {

	// ゼロベクトルを返す
	Vector3 MakeZeroVector3();

	// 右方向のベクトル
	Vector3 MakeRightVector3();
	// 上方向のベクトル
	Vector3 MakeUpVector3();
	// 前方のベクトル
	Vector3 MakeForwardVector3();

	// ベクトルの長さ
	float Length(const Vector3& a);

	// 正規化
	Vector3 Normalize(const Vector3& a);

	// ベクトルの向きを取得
	Vector3 Forward(const Vector3& rotate);

	// ワールド行列からワールド座標取得
	Vector3 ExtractionWorldPos(const Matrix4x4& m);


	// 4x4単位行列を返す
	Matrix4x4 MakeIdentityMatrix4x4();

	// 逆行列作成関数
	Matrix4x4 Inverse(const Matrix4x4& a);

	// 転置行列
	Matrix4x4 Transpose(const Matrix4x4& a);

	// 逆転置行列
	Matrix4x4 MakeInverseTransposeMatrix(const Matrix4x4& a);


	// 拡縮行列作成関数
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	// 回転行列作成関数
	Matrix4x4 MakeRotateXMatrix(float radian); // X回転
	Matrix4x4 MakeRotateYMatrix(float radian); // Y回転
	Matrix4x4 MakeRotateZMatrix(float radian); // Z回転
	Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate); // XYZ回転

	Matrix4x4 MakeRotateMatrix(const Quaternion& q);// XYZクオータニオン

	// 平行移動行列作成関数
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// アフィン変換(オイラー)
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// アフィン変換(クオータニオン)
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	// ビューポート行列作成
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 透視投影行列作成
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRaito, float nearClip, float farClip);

	// 正射影行列作成
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	// UV行列を作成
	Matrix4x4 MakeUVMatrix(const Vector2& scale, const float& rotateZ, const Vector2& translate);

	// 単位クオータニオンを返す
	Quaternion MakeIdentityQuaternion();

	// 協約クオータニオンを返す
	Quaternion Conjugate(const Quaternion& quaternion);

	// クオータニオンのnormを返す
	float Norm(const Quaternion& quaternion);

	// オイラー角をクオータニオンに変換
	Quaternion EulerToQuaternion(const Vector3& euler);

	// 正規化したクオータニオンを返す
	Quaternion Normalize(const Quaternion& quaternion);
	// 逆クオータニオンを返す
	Quaternion Inverse(const Quaternion& quaternion);
	// 任意回転軸を表すクオータニオンの生成
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	// 球面線形補完
	Quaternion Slerp(Quaternion q1, Quaternion q2, float t);
}