#pragma once

// C++
#include <cstdint>

/// <summary>
/// 3Dプリミティブ形状で汎用的に使える定数
/// </summary>
namespace PrimitiveCommonConst {
	const uint32_t kNumMaxInstance = 65535;
}

/// <summary>
/// 3D球体で使う定数
/// </summary>
namespace Sphere3DConst {
	const uint32_t maxLon = 32;
	const uint32_t maxLat = 32;
	const uint32_t maxTilesPerSphere = maxLon * maxLat;
};

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