#pragma once

// C++
#include <cstdint>

/// <summary>
/// 三角形の頂点順
/// </summary>
namespace Triangle3DVertices {
	const uint32_t LeftBottom = 0;
	const uint32_t Top = 1;
	const uint32_t RightBottom = 2;
};

/// <summary>
/// 板ポリの頂点順
/// </summary>
namespace Plane3DVertices {
	const uint32_t LeftTop = 0;
	const uint32_t RightTop = 1;
	const uint32_t LeftBottom = 2;
	const uint32_t RightBottom = 3;
};