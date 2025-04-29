#pragma once

// C++
#include <cstdint>

/// <summary>
/// ポストエフェクトパイプラインの種類
/// </summary>
enum class PostEffectType {
	Copy, // ポストエフェクトなし
	Grayscale, // グレースケール

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kPostEffectPipelineStateNum = static_cast<uint32_t>(PostEffectType::Num);
