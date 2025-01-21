#include "MathUtility.h"

#include <cassert>

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

Vector3 MAGIMath::MakeZeroVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::ExtractionWorldPos(const Matrix4x4& m) {
	Vector3 result{};
	result.x = m.m[3][0];
	result.y = m.m[3][1];
	result.z = m.m[3][2];
	return result;
}

Matrix4x4 MAGIMath::MakeIdentityMatrix4x4() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MAGIMath::Inverse(const Matrix4x4& a) {
	Matrix4x4 A = a;
	Matrix4x4 B = MakeIdentityMatrix4x4();

	int i, j, k;
	for (i = 0; i < 4; ++i) {
		if (A.m[i][i] == 0) {
			// ゼロ除算を避ける
			assert(false && "Zero Divide");
			return MakeIdentityMatrix4x4();
		}
		float scale = 1.0f / A.m[i][i];
		for (j = 0; j < 4; ++j) {
			A.m[i][j] *= scale;
			B.m[i][j] *= scale;
		}
		for (k = 0; k < 4; ++k) {
			if (k != i) {
				float factor = A.m[k][i];
				for (j = 0; j < 4; ++j) {
					A.m[k][j] -= factor * A.m[i][j];
					B.m[k][j] -= factor * B.m[i][j];
				}
			}
		}
	}

	return B;
}

Matrix4x4 MAGIMath::Transpose(const Matrix4x4& a) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = a.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MAGIMath::MakeInverseTransposeMatrix(const Matrix4x4& a) {
	Matrix4x4 result = Inverse(a);
	result = Transpose(result);
	return result;
}

Matrix4x4 MAGIMath::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 rotateX{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(radian), std::sin(radian), 0.0f,
		0.0f, -std::sin(radian), std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateX;
}

Matrix4x4 MAGIMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 rotateY{
		std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(radian), 0.0f, std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateY;
}

Matrix4x4 MAGIMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 rotateZ{
		std::cos(radian), std::sin(radian), 0.0f, 0.0f,
		-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateZ;
}

Matrix4x4 MAGIMath::MakeRotateXYZMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateX * rotateY * rotateZ;
	return result;
}

Matrix4x4 MAGIMath::MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 mat{};

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float yz = q.y * q.z;
	float zx = q.z * q.x;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	// 行 (i), 列 (j) の順で代入
	// row 0
	mat.m[0][0] = 1.0f - 2.0f * (yy + zz);
	mat.m[0][1] = 2.0f * (xy + wz);
	mat.m[0][2] = 2.0f * (zx - wy);
	mat.m[0][3] = 0.0f;

	// row 1
	mat.m[1][0] = 2.0f * (xy - wz);
	mat.m[1][1] = 1.0f - 2.0f * (xx + zz);
	mat.m[1][2] = 2.0f * (yz + wx);
	mat.m[1][3] = 0.0f;

	// row 2
	mat.m[2][0] = 2.0f * (zx + wy);
	mat.m[2][1] = 2.0f * (yz - wx);
	mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
	mat.m[2][3] = 0.0f;

	// row 3
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;
}

Matrix4x4 MAGIMath::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
	return result;
}

Matrix4x4 MAGIMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * MakeRotateXYZMatrix(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MAGIMath::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * MakeRotateMatrix(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MAGIMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result =
	{
			width / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -height / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, maxDepth - minDepth, 0.0f, left + width / 2.0f, top + height / 2.0f, minDepth, 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakePerspectiveFovMatrix(float fovY, float aspectRaito, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		(1.0f / aspectRaito) * (1.0f / std::tan(fovY / 2.0f)), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / std::tan(fovY / 2.0f), 0.0f, 0.0f,
		0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, (-nearClip * farClip) / (farClip - nearClip), 0.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f,
	};
	return result;
}

Matrix4x4 MAGIMath::MakeUVMatrix(const Vector2& scale, const float& rotateZ, const Vector2& translate) {
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotateZ);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(Vector3(scale.x, scale.y, 1.0f));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(Vector3(translate.x, translate.y, 0.0f));
	return scaleMatrix * rotateZMatrix * translateMatrix;
}

Quaternion MAGIMath::MakeIdentityQuaternion() {
	Quaternion result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;
	return result;
}

Quaternion MAGIMath::EulerToQuaternion(const Vector3& euler) {
	// オイラー角を半分にする
	float halfPitch = euler.x * 0.5f;
	float halfYaw = euler.y * 0.5f;
	float halfRoll = euler.z * 0.5f;

	// 三角関数をあらかじめ計算
	float sinX = std::sin(halfPitch);
	float cosX = std::cos(halfPitch);
	float sinY = std::sin(halfYaw);
	float cosY = std::cos(halfYaw);
	float sinZ = std::sin(halfRoll);
	float cosZ = std::cos(halfRoll);

	Quaternion q;

	// X → Y → Z 回転順序に基づくクオータニオンの計算
	q.w = cosX * cosY * cosZ - sinX * sinY * sinZ;
	q.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
	q.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
	q.z = cosX * cosY * sinZ + sinX * sinY * cosZ;

	return q;
}