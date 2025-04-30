#pragma once

#include "Enums/PostEffectPipelineEnum.h"

/// <summary>
/// パラメータ
/// </summary>
struct PostEffectParamater {
	float param0 = 0.0f;
	float param1 = 0.0f;
	float param2 = 0.0f;
	float param3 = 0.0f;
};

struct PostEffectCommand {
	PostEffectType postEffectType;
	PostEffectParamater param;
};