#pragma once

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

/// <summary>
/// ブレンドモードの種類
/// </summary>
enum class BlendMode {
	None,
	Normal,
	Add,
	Subtract,
	Multiply,
	Screen,

	Num,
};
