#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>
#include <array>

// MyHedder
#include "Math/Utility/MathUtility.h"

#include "Structs/AnimationStruct.h"
#include "Structs/ColorStruct.h"

/// <summary>
/// 便利関数
/// </summary>
namespace MAGIUtility {

	// Vector3のキーフレーム補間
	Vector3 CalculateVelue(const std::vector<KeyframeVector3>& keyframes, float time);
	// Quaternionのキーフレーム補間
	Quaternion CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time);

	// ガウスの重みを生成する
	std::array<float, 7> GenerateGaussianWeights(float sigma);

	// HashとEqualを透過対応にする
	struct TransparentHash {
		using is_transparent = void;
		size_t operator()(std::string_view key) const {
			return std::hash<std::string_view>{}(key);
		}
	};

	struct TransparentEqual {
		using is_transparent = void;
		bool operator()(std::string_view lhs, std::string_view rhs) const {
			return lhs == rhs;
		}
	};
}