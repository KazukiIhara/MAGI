#pragma once

#include <string>
#include <format>

// 
// デバッグログ表示用クラス
// 

class Logger {
public: // 静的メンバ関数
	// デバッグログの出力
	static void Log(const std::string& message);
	// string型をwstring型に変換する関数
	static std::wstring ConvertString(const std::string& str);
	// wstring型をstring型に変換する関数
	static std::string ConvertString(const std::wstring& str);
};