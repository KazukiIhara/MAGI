#pragma once

// C++
#include <cstdint>

/// <summary>
/// シャドウ用のパイプラインの種類
/// </summary>
enum class ShadowPipelineStateType {
	Model3D,

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kShadowPipelineStateNum = static_cast<uint32_t>(ShadowPipelineStateType::Num);
