#pragma once

// C++
#include <cmath>
#include <cassert>
#include <memory>

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "Enums/BlendModeEnum.h"
#include "Enums/PostEffectPipelineEnum.h"
#include "Structs/ColorStruct.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class RTVManager;
class SRVUAVManager;

/// <summary>
/// レンダーテクスチャマネージャ
/// </summary>
class RenderTextureManager {
public:
	RenderTextureManager();
	~RenderTextureManager();

private:

};