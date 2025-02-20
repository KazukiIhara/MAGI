#pragma once

// C++
#include <iostream>
#include <string>

/// <summary>
/// 当たり判定のカテゴリ
/// </summary>
enum class CollisionCategory {
	None,
	Player,
	Enemy,


	Count,
};

/// <summary>
/// 当たり判定のカテゴリを文字列型に変換する
/// </summary>
/// <param name="category">カテゴリ</param>
/// <returns>カテゴリの文字列型</returns>
inline std::string CollisionCategoryToString(CollisionCategory category) {
	switch (category) {
	case CollisionCategory::None:
		return "None";
	case CollisionCategory::Player:
		return "Player";
	case CollisionCategory::Enemy:
		return "Enemy";
	default:
		return "Unknown";
	}
}