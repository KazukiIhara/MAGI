#pragma once

#include <cstdint>

#include "Math/Types/Vector3.h"

#include "Structs/ColorStruct.h"
#include "Enums/EmitterEnum.h" 

/// <summary>
/// エミッターの設定構造体
/// </summary>
struct EmitterSetting {
	// 発生のタイプ
	EmitType emitType = EmitType::Default;
	// 発生場所
	Vector3 minTranslate{};
	Vector3 maxTranslate{};
	// 移動量
	Vector3	minVelocity{};
	Vector3 maxVelocity{};
	// サイズ
	float minScale;
	float maxScale;
	// 色
	RGBA minColor{};
	RGBA maxColor{};
	// 生存時間
	float minLifeTime;
	float maxLifeTime;
	// 一度の発生個数
	uint32_t count;
	// 繰り返し発生するかどうか
	bool isRepeat;
	// 発生頻度
	float frequency;
};