#include "MathUtility.h"

Vector2 operator-(const Vector2& v) {
	return Vector2(-v.x, -v.y);
}

Vector2 operator+(const Vector2& v) {
	return v;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator*(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x * v2.x, v1.y * v2.y);
}

Vector2 operator/(const Vector2& v1, const Vector2& v2) {
	return Vector2(v1.x / v2.x, v1.y / v2.y);
}

Vector2 operator/(const Vector2& v, float s) {
	return Vector2(v.x / s, v.y / s);
}

Vector2 operator*(float s, const Vector2& v) {
	return Vector2(v.x * s, v.y * s);
}

Vector2 operator*(const Vector2& v, float s) {
	return Vector2(v.x * s, v.y * s);
}

///
/// Vector3
///

Vector3 operator-(const Vector3& v) {
	return Vector3(-v.x, -v.y, -v.z);
}

Vector3 operator+(const Vector3& v) {
	return v;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vector3 operator/(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

Vector3 operator*(float s, const Vector3& v) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator*(const Vector3& v, float s) {
	return Vector3(v.x * s, v.y * s, v.z * s);
}

Vector3 operator/(const Vector3& v, float s) {
	return Vector3(v.x / s, v.y / s, v.z / s);
}

///
/// Vector4
///

Vector4 operator-(const Vector4& v) {
	return Vector4(-v.x, -v.y, -v.z, -v.w);
}

Vector4 operator+(const Vector4& v) {
	return v;
}

Vector4 operator+(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

Vector4 operator-(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Vector4 operator*(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

Vector4 operator/(const Vector4& v1, const Vector4& v2) {
	return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

Vector4 operator*(float s, const Vector4& v) {
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4 operator*(const Vector4& v, float s) {
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4 operator/(const Vector4& v, float s) {
	return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);
}

///
/// Matrix4x4
///

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

float MAGIMath::Lerp(float a, float b, float t) {
	return a + t * (b - a);
}

float MAGIMath::DegreesToRadians(const float& degrees) {
	return degrees * (std::numbers::pi_v<float> / 180.0f);
}

float MAGIMath::Dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float MAGIMath::Length(const Vector3& a) {
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3 MAGIMath::Normalize(const Vector3& a) {
	float length = Length(a);
	Vector3 normalizedVector{};

	if (length != 0) {
		normalizedVector.x = a.x / length;
		normalizedVector.y = a.y / length;
		normalizedVector.z = a.z / length;
	} else {
		normalizedVector.x = 0;
		normalizedVector.y = 0;
		normalizedVector.z = 0;
	}

	return normalizedVector;
}

Vector3 MAGIMath::DegreesToRadians(const Vector3& degrees) {
	Vector3 result{};
	result.x = DegreesToRadians(degrees.x);
	result.y = DegreesToRadians(degrees.y);
	result.z = DegreesToRadians(degrees.z);
	return result;
}

Vector3 MAGIMath::ExtractionWorldPos(const Matrix4x4& m) {
	Vector3 worldPos{};
	worldPos.x = m.m[3][0];
	worldPos.y = m.m[3][1];
	worldPos.z = m.m[3][2];
	return worldPos;
}

Vector3 MAGIMath::CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
	int numPoints = static_cast<int>(controlPoints.size());
	if (numPoints < 4) {
		// 4つ未満のポイントではCatmull-Romスプラインが機能しないため、直接返す
		return controlPoints[0];
	}

	// ttの計算
	float tt = t * (numPoints - 1);
	int p1 = static_cast<int>(tt);
	p1 = std::clamp(p1, 1, numPoints - 3); // p1を範囲内に制限
	int p0 = p1 - 1;
	int p2 = p1 + 1;
	int p3 = p2 + 1;

	tt = tt - p1; // 局所的なtを取得

	// Catmull-Romスプラインの計算式に基づく補間
	float tt2 = tt * tt;
	float tt3 = tt2 * tt;

	Vector3 a = controlPoints[p0] * (-0.5f) + controlPoints[p1] * (1.5f) - controlPoints[p2] * (1.5f) + controlPoints[p3] * (0.5f);
	Vector3 b = controlPoints[p0] - controlPoints[p1] * (2.5f) + controlPoints[p2] * (2.0f) - controlPoints[p3] * (0.5f);
	Vector3 c = controlPoints[p2] * 0.5f - controlPoints[p0] * 0.5f;
	Vector3 d = controlPoints[p1];

	return a * tt3 + b * tt2 + c * tt + d;
}

Vector3 MAGIMath::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Vector3(
		v1.x + t * (v2.x - v1.x),
		v1.y + t * (v2.y - v1.y),
		v1.z + t * (v2.z - v1.z)
	);
}

Vector3 MAGIMath::Cross(const Vector3& a, const Vector3& b) {
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

Vector3 MAGIMath::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	Vector4 coord = { vector.x, vector.y, vector.z, 1.0f };
	Vector4 temp{};
	temp.x = coord.x * matrix.m[0][0] + coord.y * matrix.m[1][0] + coord.z * matrix.m[2][0] + coord.w * matrix.m[3][0];
	temp.y = coord.x * matrix.m[0][1] + coord.y * matrix.m[1][1] + coord.z * matrix.m[2][1] + coord.w * matrix.m[3][1];
	temp.z = coord.x * matrix.m[0][2] + coord.y * matrix.m[1][2] + coord.z * matrix.m[2][2] + coord.w * matrix.m[3][2];
	temp.w = coord.x * matrix.m[0][3] + coord.y * matrix.m[1][3] + coord.z * matrix.m[2][3] + coord.w * matrix.m[3][3];

	result.x = temp.x / temp.w;
	result.y = temp.y / temp.w;
	result.z = temp.z / temp.w;

	return result;
}
