#include "DataIO.h"

#include "Renderer3DManager/Renderer3DManager.h"
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

DataIO::DataIO(Renderer3DManager* renderer3DManager, ColliderManager* colliderManager) {
	// 
	// インスタンスのセット
	// 
	SetRenderer3DManager(renderer3DManager);
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

void DataIO::LoadRenderer3DDataFile(const std::string& fileName) {
	// パスを組み立て (例: "Assets/Datas/Renderer3D/<fileName>")
	std::filesystem::path directoryPath = "Assets/Datas/Renderer3D";
	std::filesystem::path inputPath = directoryPath / fileName;

	// 拡張子を強制的に .json にする
	inputPath.replace_extension(".json");

	// ファイルが存在するかチェック
	if (!std::filesystem::exists(inputPath)) {
		Logger::Log("File not found: " + inputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("ファイルが見つかりません: " + inputPath.generic_string());
		std::wstring wTitle = Logger::ConvertString("Load Renderer3D Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// ファイルを開く
	std::ifstream ifs(inputPath);
	if (!ifs) {
		Logger::Log("Failed to open file: " + inputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("ファイルを開けませんでした: " + inputPath.generic_string());
		std::wstring wTitle = Logger::ConvertString("Load Renderer3D Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// JSON パース
	json jsonData;
	try {
		ifs >> jsonData;
	}
	catch (const std::exception& e) {
		Logger::Log(std::string("JSON parse error: ") + e.what() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("JSONの読み込みに失敗しました:\n");
		wMessage += Logger::ConvertString(e.what());
		std::wstring wTitle = Logger::ConvertString("Load Renderer3D Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// Renderer3Dデータが含まれているか確認
	if (!jsonData.contains("Renderer3D") || !jsonData["Renderer3D"].is_object()) {
		Logger::Log("No 'Renderer3D' object in JSON: " + inputPath.string() + "\n");
		return;
	}

	// 現時点ではパラメータのロードはなし
	Logger::Log("Renderer3D data loaded from: " + inputPath.string() + "\n");

#ifdef _WIN32
	// 読み込み完了メッセージ
	std::wstring wMessage = Logger::ConvertString("Renderer3D情報をロードしました。\n" + inputPath.generic_string());
	std::wstring wTitle = Logger::ConvertString("Load Renderer3D Data");
	MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONINFORMATION);
#endif

}

void DataIO::LoadColliderDataFile(const std::string& fileName) {
	// コライダーのリストをクリア（既存コライダー削除）
	colliderManager_->Clear();

	// パスを組み立て (例: "Assets/Datas/Colliders/<fileName>")
	std::filesystem::path directoryPath = "Assets/Datas/Colliders";
	std::filesystem::path inputPath = directoryPath / fileName;

	// ファイルが存在するかチェック
	if (!std::filesystem::exists(inputPath)) {
		Logger::Log("File not found: " + inputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("ファイルが見つかりません: " + inputPath.generic_string());
		std::wstring wTitle = Logger::ConvertString("Load Collider Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// ファイルを開く
	std::ifstream ifs(inputPath);
	if (!ifs) {
		Logger::Log("Failed to open file: " + inputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("ファイルを開けませんでした: " + inputPath.generic_string());
		std::wstring wTitle = Logger::ConvertString("Load Collider Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// JSON パース
	json jsonData;
	try {
		ifs >> jsonData;
	}
	catch (const std::exception& e) {
		Logger::Log(std::string("JSON parse error: ") + e.what() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString("JSONの読み込みに失敗しました:\n");
		wMessage += Logger::ConvertString(e.what());
		std::wstring wTitle = Logger::ConvertString("Load Collider Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// コライダーデータが含まれているか確認
	if (!jsonData.contains("Colliders") || !jsonData["Colliders"].is_array()) {
		Logger::Log("No 'Colliders' array in JSON: " + inputPath.string() + "\n");
		return;
	}

	// JSON 配列を走査してコライダーを生成
	auto& collidersArray = jsonData["Colliders"];
	for (auto& colliderJson : collidersArray) {
		// 必要なフィールドがあるかチェック
		if (!colliderJson.contains("name") || !colliderJson.contains("type")) {
			Logger::Log("Invalid collider entry: missing 'name' or 'type'.\n");
			continue;
		}

		// それぞれ読み取る
		std::string colliderName = colliderJson["name"].get<std::string>();
		int typeValue = colliderJson["type"].get<int>();

		// enum class へキャスト
		Collider3DType colliderType = static_cast<Collider3DType>(typeValue);

		// コライダーを生成 (ColliderManager 内のメソッドに合わせて修正)
		colliderManager_->Create(colliderName, colliderType);

		// 作成したコライダーを検索
		BaseCollider3D* newCollider = colliderManager_->Find(colliderName);

		if (!newCollider) {
			Logger::Log("Failed to create collider: " + colliderName + "\n");
			continue;
		}

		// ------------------------------------------------
		// offset (x, y, z)
		// ------------------------------------------------
		if (colliderJson.contains("offset")) {
			auto offsetJson = colliderJson["offset"];
			float x = offsetJson.value("x", 0.0f);
			float y = offsetJson.value("y", 0.0f);
			float z = offsetJson.value("z", 0.0f);
			newCollider->GetOffset() = { x, y, z };
		}

		// ------------------------------------------------
		// radius (Sphere のみ)
		// ------------------------------------------------
		if (colliderType == Collider3DType::Sphere) {
			auto sphereCollider = dynamic_cast<SphereCollider*>(newCollider);
			if (sphereCollider && colliderJson.contains("radius")) {
				float radiusVal = colliderJson["radius"].get<float>();
				sphereCollider->GetRadius() = radiusVal;
			}
		}

		// ------------------------------------------------
		// category (文字列から列挙型に変換)
		// ------------------------------------------------
		if (colliderJson.contains("category")) {
			std::string categoryStr = colliderJson["category"].get<std::string>();
			CollisionCategory category = CollisionCategory::None; // デフォルト値

			// 文字列からカテゴリーを取得
			category = StringToCollisionCategory(categoryStr);

			// カテゴリを設定
			newCollider->GetCategory() = category;
		}

		Logger::Log("Collider loaded: " + colliderName + " (type=" + std::to_string(typeValue) + ")\n");
	}

	Logger::Log("Collider data loaded from: " + inputPath.string() + "\n");
#ifdef _WIN32
	std::wstring wMessage = Logger::ConvertString("コライダー情報をロードしました。\n" + inputPath.generic_string());
	std::wstring wTitle = Logger::ConvertString("Load Collider Data");
	MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONINFORMATION);
#endif
}


void DataIO::SaveRenderer3DDataFile(const std::string& fileName) {
	// 出力先ディレクトリ
	std::filesystem::path directoryPath = "Assets/Datas/Renderer3D";
	// ファイル名を連結してフルパスを作る
	std::filesystem::path outputPath = directoryPath / fileName;

	// 拡張子を強制的に .json にする
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
			std::wstring wTitle = Logger::ConvertString("Save Renderer3D Data");
			MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
			return;
		}
	}

	// JSON オブジェクトを用意
	json jsonData;
	jsonData["Renderer3D"] = json::array();

	// Renderer3DManager からオブジェクトリストを取得
	const auto& objects = renderer3DManager_->GetRenderers();

	// オブジェクト情報を JSON に追加
	for (const auto& obj : objects) {
		if (!obj) {
			continue;
		}

		// JSON に名前を追加
		json objectJson;
		objectJson["name"] = obj->name_; // オブジェクト名を取得

		jsonData["Renderer3D"].push_back(objectJson);
	}

	// ファイルを書き込み用に開く
	std::ofstream ofs(outputPath, std::ios::out | std::ios::trunc);
	if (!ofs) {
		Logger::Log("Failed to open file: " + outputPath.string() + "\n");
#ifdef _WIN32
		std::wstring wMessage = Logger::ConvertString(
			"ファイルのオープンに失敗しました。\n" + outputPath.generic_string()
		);
		std::wstring wTitle = Logger::ConvertString("Save Renderer3D Data");
		MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONERROR);
#endif
		return;
	}

	// JSON を整形して書き込み (インデント = 4)
	ofs << jsonData.dump(4);
	ofs.close();

	// 保存完了をログ出力
	Logger::Log("Renderer3D data saved to: " + outputPath.string() + "\n");

#ifdef _WIN32
	// メッセージボックスで表示 (ワイド文字に変換し、区切りを '/' に統一した文字列を使用)
	std::wstring wMessage = Logger::ConvertString(
		"Renderer3D情報の保存が完了しました。\n" + outputPath.generic_string()
	);
	std::wstring wTitle = Logger::ConvertString("Save Renderer3D Data");
	MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONINFORMATION);
#endif
}


void DataIO::SaveColliderDataFile(const std::string& fileName) {
	// 出力先ディレクトリ
	std::filesystem::path directoryPath = "Assets/Datas/Colliders";
	// ファイル名を連結してフルパスを作る
	std::filesystem::path outputPath = directoryPath / fileName;

	// 拡張子を強制的に .json にする
	outputPath.replace_extension(".json");

	// ディレクトリが存在しない場合は作成 (再帰的に作成する)
	if (!std::filesystem::exists(directoryPath)) {
		std::error_code ec;
		std::filesystem::create_directories(directoryPath, ec);
		if (ec) {
			Logger::Log("Failed to create directory: " + directoryPath.string() + "\n");
#ifdef _WIN32
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

		// 基本情報
		json colliderJson;
		colliderJson["name"] = collider->name_;
		colliderJson["type"] = static_cast<int>(collider->GetType());
		// カテゴリを文字列で保存
		colliderJson["category"] = CollisionCategoryToString(collider->GetCategory());

		// オフセットの追加 (Vector3 などを想定)
		{
			auto offset = collider->GetOffset();
			colliderJson["offset"] = {
				{"x", offset.x},
				{"y", offset.y},
				{"z", offset.z}
			};
		}

		// 球体コライダーの場合は半径を追加
		if (collider->GetType() == Collider3DType::Sphere) {
			if (auto sphereCollider = dynamic_cast<SphereCollider*>(collider.get())) {
				colliderJson["radius"] = sphereCollider->GetRadius();
			}
		}

		jsonData["Colliders"].push_back(colliderJson);
	}

	// ファイルを書き込み用に開く
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
	std::wstring wMessage = Logger::ConvertString(
		"コライダー情報の保存が完了しました。\n" + outputPath.generic_string()
	);
	std::wstring wTitle = Logger::ConvertString("Save Collider Data");
	MessageBoxW(nullptr, wMessage.c_str(), wTitle.c_str(), MB_OK | MB_ICONINFORMATION);
#endif
}

Renderer3DManager* DataIO::GetRenderer3DManager() {
	return renderer3DManager_;
}

ColliderManager* DataIO::GetColliderManager() {
	return colliderManager_;
}

void DataIO::SetRenderer3DManager(Renderer3DManager* renderer3DManager) {
	assert(renderer3DManager);
	renderer3DManager_ = renderer3DManager;
}

void DataIO::SetColliderManager(ColliderManager* colliderManager) {
	assert(colliderManager);
	colliderManager_ = colliderManager;
}
