#include "MathUtility.h"

#include <cassert>
#include <stdexcept>

static const float EPSILON = 1.0e-6f;

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
	Matrix4x4 result{};
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

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
	);
}

Vector3 MAGIMath::MakeZeroVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeRightVector3() {
	Vector3 result{};
	result.x = 1.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeUpVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 1.0f;
	result.z = 0.0f;
	return result;
}

Vector3 MAGIMath::MakeForwardVector3() {
	Vector3 result{};
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 1.0f;
	return result;
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

Vector3 MAGIMath::Forward(const Vector3& rotate) {
	// 基準の前方向ベクトル (ワールド座標系の Z 軸正方向)
	Vector3 baseForward = MakeForwardVector3();

	// XYZ軸の回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(rotate);

	// 回転行列で基準ベクトルを変換
	Vector3 forward{};
	forward.x = rotationMatrix.m[0][0] * baseForward.x +
		rotationMatrix.m[1][0] * baseForward.y +
		rotationMatrix.m[2][0] * baseForward.z;

	forward.y = rotationMatrix.m[0][1] * baseForward.x +
		rotationMatrix.m[1][1] * baseForward.y +
		rotationMatrix.m[2][1] * baseForward.z;

	forward.z = rotationMatrix.m[0][2] * baseForward.x +
		rotationMatrix.m[1][2] * baseForward.y +
		rotationMatrix.m[2][2] * baseForward.z;

	// 単位ベクトル化
	return Normalize(forward);
}

Vector3 MAGIMath::Transform(const Vector3& vector, const Quaternion& rotation) {
	// クォータニオンの逆（共役）を計算
	Quaternion conjugateRotation = Conjugate(rotation);

	// ベクトルをクォータニオン形式に変換（w = 0）
	Quaternion vectorQuat = { vector.x, vector.y, vector.z, 0.0f };

	// 回転の適用: q * v * q^-1
	Quaternion rotatedQuat = rotation * vectorQuat * conjugateRotation;

	// 結果をベクトル形式に戻す
	return { rotatedQuat.x, rotatedQuat.y, rotatedQuat.z };
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

	for (int i = 0; i < 4; ++i) {
		if (std::abs(A.m[i][i]) < 1e-6f) {
			bool swapped = false;
			for (int row = i + 1; row < 4; ++row) {
				if (std::abs(A.m[row][i]) > 1e-6f) {
					std::swap(A.m[i], A.m[row]);
					std::swap(B.m[i], B.m[row]);
					swapped = true;
					break;
				}
			}
			if (!swapped) {
				throw std::runtime_error("Matrix is singular and cannot be inverted.");
			}
		}

		float scale = 1.0f / A.m[i][i];
		for (int j = 0; j < 4; ++j) {
			A.m[i][j] *= scale;
			B.m[i][j] *= scale;
		}

		for (int k = 0; k < 4; ++k) {
			if (k != i) {
				float factor = A.m[k][i];
				for (int j = 0; j < 4; ++j) {
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

Quaternion MAGIMath::Conjugate(const Quaternion& quaternion) {
	Quaternion result{};
	result.x = -quaternion.x;
	result.y = -quaternion.y;
	result.z = -quaternion.z;
	result.w = quaternion.w;
	return result;
}

float MAGIMath::Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
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

Quaternion MAGIMath::Normalize(const Quaternion& quaternion) {
	// ノルムを求める
	float length = Norm(quaternion);

	// ノルムが 0 の場合は正規化できない
	if (length == 0.0f) {
		throw std::runtime_error("Cannot normalize a zero-norm quaternion.");
	}

	// 逆数を計算
	float inv_length = 1.0f / length;

	// 正規化したクオータニオンを返す
	Quaternion result;
	result.x = quaternion.x * inv_length;
	result.y = quaternion.y * inv_length;
	result.z = quaternion.z * inv_length;
	result.w = quaternion.w * inv_length;
	return result;
}

Quaternion MAGIMath::Inverse(const Quaternion& quaternion) {
	float norm_sq =
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;

	if (norm_sq == 0.0f) {
		// ノルムが0の場合は逆が定義できないため例外を投げる
		throw std::runtime_error("Cannot invert a zero-norm quaternion.");
	}

	// 協約
	Quaternion conj = Conjugate(quaternion);

	// 1 / ノルム^2
	float inv_norm = 1.0f / norm_sq;

	// 結果を格納
	Quaternion result;
	result.x = conj.x * inv_norm;
	result.y = conj.y * inv_norm;
	result.z = conj.z * inv_norm;
	result.w = conj.w * inv_norm;

	return result;
}

Quaternion MAGIMath::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 軸ベクトルの長さを求める
	float axis_len = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	// 軸ベクトルがゼロ長なら回転を定義できないので、適宜例外を投げる
	if (axis_len == 0.0f) {
		throw std::runtime_error("Axis vector has zero length. Cannot create rotation quaternion.");
	}

	// 正規化した軸ベクトル
	float inv_len = 1.0f / axis_len;
	float nx = axis.x * inv_len;
	float ny = axis.y * inv_len;
	float nz = axis.z * inv_len;

	// 回転角を2で割った値
	float half_angle = angle * 0.5f;
	float s = std::sin(half_angle);
	float c = std::cos(half_angle);

	// クオータニオン (x, y, z, w) の順で格納
	Quaternion q{};
	q.x = nx * s;
	q.y = ny * s;
	q.z = nz * s;
	q.w = c;

	return q;
}

Quaternion MAGIMath::Slerp(Quaternion q1, Quaternion q2, float t) {
	// クォータニオンの内積を計算
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	// ドット積が負の場合、逆の方向に補間するために q1 を反転
	if (dot < 0.0f) {
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
		dot = -dot;
	}

	if (dot >= 1.0f - EPSILON) {
		Quaternion result{};
		result.x = (1.0f - t) * q1.x + t * q2.x;
		result.y = (1.0f - t) * q1.y + t * q2.y;
		result.z = (1.0f - t) * q1.z + t * q2.z;
		result.w = (1.0f - t) * q1.w + t * q2.w;

		return Normalize(result);
	}

	// 角度の計算
	float theta_0 = std::acos(dot);        // θ0 = q1 と q2 間の角度
	float theta = theta_0 * t;             // θ = t に対応する角度

	// 係数の計算
	float sin_theta = std::sin(theta);
	float sin_theta_0 = std::sin(theta_0);

	float s1 = std::cos(theta) - dot * sin_theta / sin_theta_0;
	float s2 = sin_theta / sin_theta_0;

	// 補間結果の計算
	Quaternion result = {
		s1 * q1.x + s2 * q2.x,
		s1 * q1.y + s2 * q2.y,
		s1 * q1.z + s2 * q2.z,
		s1 * q1.w + s2 * q2.w
	};
	return Normalize(result);
}
