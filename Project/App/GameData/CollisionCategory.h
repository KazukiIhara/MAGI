#pragma once

// C++
#include <iostream>
#include <string>
#include <vector>

/// <summary>
/// 当たり判定のカテゴリ
/// </summary>
enum class CollisionCategory {
	None,

	Count,
};

// TODO: テンプレートを使ってもっとスマートな実装をする

/// <summary>
/// 当たり判定のカテゴリを文字列型に変換する
/// </summary>
/// <param name="category">カテゴリ</param>
/// <returns>カテゴリの文字列型</returns>
inline std::string CollisionCategoryToString(CollisionCategory category) {
	switch (category) {
	case CollisionCategory::None:
		return "None";
	default:
		return "Unknown";
	}
}

/// <summary>
/// 文字列から当たり判定カテゴリに変換する
/// </summary>
/// <param name="categoryName">カテゴリ文字列</param>
/// <returns>カテゴリ</returns>
inline CollisionCategory StringToCollisionCategory(const std::string& categoryName) {
	if (categoryName == "None") {
		return CollisionCategory::None;
	} else {
		return CollisionCategory::None;
	}
}

// CollisionCategory の列挙値から ImGui 用の文字列配列（const char* の vector）を生成するヘルパー関数
inline const std::vector<const char*>& GetCollisionCategoryList() {
	// 文字列保持用と、ImGui 用の配列を静的変数で保持
	static std::vector<std::string> categoryNamesStr;
	static std::vector<const char*> categoryNamesCStr;
	static bool initialized = false;

	if (!initialized) {
		// CollisionCategory::Count までループして、各カテゴリの文字列を取得
		for (int i = 0; i < static_cast<int>(CollisionCategory::Count); ++i) {
			categoryNamesStr.push_back(CollisionCategoryToString(static_cast<CollisionCategory>(i)));
		}
		// 各 std::string の c_str() を取得して vector に格納
		for (const auto& str : categoryNamesStr) {
			categoryNamesCStr.push_back(str.c_str());
		}
		initialized = true;
	}

	return categoryNamesCStr;
}