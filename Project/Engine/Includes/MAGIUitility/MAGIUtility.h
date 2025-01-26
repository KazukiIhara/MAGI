#pragma once

#include "Math/Utility/MathUtility.h"

#include "Structs/AnimationStruct.h"

/// <summary>
/// 便利関数
/// </summary>
namespace MAGIUtility {

	// Vector3のキーフレーム補間
	Vector3 CalculateVelue(const std::vector<KeyframeVector3>& keyframes, float time);
	// Quaternionのキーフレーム補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

}