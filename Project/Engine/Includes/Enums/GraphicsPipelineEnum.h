#pragma once

// C++
#include <cstdint>

/// <summary>
/// グラフィックスパイプラインの種類
/// </summary>
enum class GraphicsPipelineStateType {
	Object3D,
	Object3DNormalMap,

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kGraphicsPipelineStateNum = static_cast<uint32_t>(GraphicsPipelineStateType::Num);
