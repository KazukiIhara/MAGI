#pragma once

// C++
#include <cstdint>

/// <summary>
/// グラフィックスパイプラインの種類
/// </summary>
enum class GraphicsPipelineStateType {
	Object2D,
	Object3D,
	Particle,
	Line,

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kGraphicsPipelineStateNum = static_cast<uint32_t>(GraphicsPipelineStateType::Num);
