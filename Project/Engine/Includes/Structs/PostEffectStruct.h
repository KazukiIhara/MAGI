#pragma once

#include "Enums/PostEffectPipelineEnum.h"

/// <summary>
/// パラメータ
/// </summary>
struct PostEffectParamater {
	float param[4];
};

struct PostEffectCommand {
	PostEffectType postEffectType;
	PostEffectParamater param;
};