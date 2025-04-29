#pragma once

#include "Enums/PostEffectPipelineEnum.h"

/// <summary>
/// パラメータ
/// </summary>
struct PostEffectParamater {
	float param0;
	float param1;
	float param2;
	float param3;
};

struct PostEffectCommand {
	PostEffectType postEffectType;
	float param0;
	float param1;
	float param2;
	float param3;
};