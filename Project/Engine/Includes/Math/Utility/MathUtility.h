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


/// <summary>
/// 数学関数群　(左手座標系、行ベクトル)
/// </summary>
namespace MAGIMath {

	// 線形補完
	float Lerp(float a, float b, float t);

	// 度数法をラジアンに変換
	float DegreesToRadians(const float& degrees);

	// 内積
	float Dot(const Vector3& a, const Vector3& b);

	// ベクトルの長さ
	float Length(const Vector3& a);

	// 正規化
	Vector3 Normalize(const Vector3& a);

	// 度数法をラジアンに変換
	Vector3 DegreesToRadians(const Vector3& degrees);

	// ワールド行列からワールド座標取得
	Vector3 ExtractionWorldPos(const Matrix4x4& m);

	// スプライン曲線補間
	Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t);

	// 線形補完
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

	// クロス積
	Vector3 Cross(const Vector3& a, const Vector3& b);

	// トランスフォーム
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

}