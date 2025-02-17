#include "DataIO.h"

#include "ColliderManager/ColliderManager.h"
#include "Logger/Logger.h"

#include <fstream>
#include <filesystem>
#include <cassert>

// Json
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Windows環境限定
#ifdef _WIN32
#include <Windows.h>
#endif

DataIO::DataIO(ColliderManager* colliderManager) {
	// 
	// インスタンスのセット
	// 
	SetColliderManager(colliderManager);

	// 
	// 初期化
	// 
	Initialize();
	Logger::Log("DataIO Initialize\n");
}

DataIO::~DataIO() {
	Logger::Log("DataIO Finalize\n");
}

void DataIO::Initialize() {

}

void DataIO::BeginFrame() {

}

void DataIO::EndFrame() {

}

void DataIO::LoadColliderDataFile(const std::string& fileName) {
	fileName;
}

void DataIO::SaveColliderDataFile(const std::string& fileName) {
	// 出力先ディレクトリ
	std::filesystem::path directoryPath = "Assets/Datas/Colliders";
	// ファイル名を連結してフルパスを作る
	std::filesystem::path outputPath = directoryPath / fileName;

	// 拡張子を強制的に .json にする
	// （拡張子がついていなくても、何か別の拡張子がついていても .json に差し替え）
	outputPath.replace_extension(".json");

	// ディレクトリが存在しない場合は作成 (再帰的に作成する)
	if (!std::filesystem::exists(directoryPath)) {
		std::error_code ec;
		std::filesystem::create_directories(directoryPath, ec);

		if (ec) {
			// ログを出して
			Logger::Log("Failed to create directory: " + directoryPath.string() + "\n");
#ifdef _WIN32
			// メッセージボックス (ワイド文字列に変換)
			std::wstring wMessage = Logger::ConvertString(
				"ディレクトリの作成に失敗しました。\n" + directoryPath.generic_string()
			);
			std::wstring wTitle = Logger::ConvertString("Save Collider Data");
			MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
			return;
		}
	}

	// ColliderManager からコライダーリストを取得
	const auto& colliders = colliderManager_->GetColliders();

	// JSON オブジェクトを用意
	json jsonData;
	jsonData["Colliders"] = json::array();

	// コライダー情報を JSON に追加
	for (const auto& collider : colliders) {
		if (!collider) {
			continue;
		}

		json colliderJson;
		colliderJson["name"] = collider->name_;
		colliderJson["type"] = static_cast<int>(collider->GetType());
		jsonData["Colliders"].push_back(colliderJson);
	}

	// ファイルを書き込み用に開く (なければ作成、あれば上書き)
	std::ofstream ofs(outputPath, std::ios::out | std::ios::trunc);
	if (!ofs) {
		Logger::Log("Failed to open file: " + outputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString(
			"ファイルのオープンに失敗しました。\n" + outputPath.generic_string()
		);
		std::wstring wTitle = Logger::ConvertString("Save Collider Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// JSON を整形して書き込み (インデント = 4)
	ofs << jsonData.dump(4);
	ofs.close();

	// 保存完了をログ出力
	Logger::Log("Collider data saved to: " + outputPath.string() + "\n");

#ifdef _WIN32
	// メッセージボックスで表示 (ワイド文字に変換し、区切りを '/' に統一した文字列を使用)
	std::wstring wMessage = Logger::ConvertString(
		"コライダー情報の保存が完了しました。\n" + outputPath.generic_string()
	);
	std::wstring wTitle = Logger::ConvertString("Save Collider Data");
	MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONINFORMATION);
#endif
}

ColliderManager* DataIO::GetColliderManager() {
	return colliderManager_;
}

void DataIO::SetColliderManager(ColliderManager* colliderManager) {
	assert(colliderManager);
	colliderManager_ = colliderManager;
}
