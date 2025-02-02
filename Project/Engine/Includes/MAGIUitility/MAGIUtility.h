#pragma once

#include "Math/Utility/MathUtility.h"

#include "Structs/AnimationStruct.h"
#include "Structs/ColorStruct.h"

/// <summary>
/// 便利関数
/// </summary>
namespace MAGIUtility {
	// RGBAからVector4に変換
	Vector4 RGBAToVector4(const RGBA& color);

	// Vector4からRGBAに変換
	RGBA Vector4ToRGBA(const Vector4& color);

	// Vector3のキーフレーム補間
	Vector3 CalculateVelue(const std::vector<KeyframeVector3>& keyframes, float time);
	// Quaternionのキーフレーム補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

}