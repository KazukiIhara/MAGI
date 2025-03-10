#pragma once

// C++
#include <cstdint>

/// <summary>
/// ポストエフェクトパイプラインの種類
/// </summary>
enum class PostEffectPipelineStateType {
	None, // ポストエフェクトなし
	Grayscale, // グレースケール

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kPostEffectPipelineStateNum = static_cast<uint32_t>(PostEffectPipelineStateType::Num);
