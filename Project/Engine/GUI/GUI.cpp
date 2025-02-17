#include "GUI.h"

#include <cassert>

#include "imgui/imgui.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "DataIO/DataIO.h"
#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"
#include "3D/GameObject3D/GameObject3D.h"

GUI::GUI(
	DeltaTimer* deltaTimer,
	SRVUAVManager* srvUavManager,
	DataIO* dataIO,
	TextureDataContainer* textureDataContainer
) {

	// 
	// インスタンスのセット
	// 
	assert(deltaTimer);
	assert(srvUavManager);
	assert(dataIO);
	assert(textureDataContainer);
	deltaTimer_ = deltaTimer;
	srvUavManager_ = srvUavManager;
	dataIO_ = dataIO;
	textureDataContainer_ = textureDataContainer;

}

void GUI::Update() {
	//
	// デバッグ描画
	//
	ShowFPS();
	ShowDeltaTime();

	//
	// アセットコンテナの描画
	//

	// テクスチャ
	ShowTextureDatas();


	// 
	// オブジェクトマネージャの描画
	// 

	// コライダーマネージャ
	ShowColliderManager();

}

void GUI::ShowFPS() {
	// フレームレート表示の設定
	ImGuiIO& io = ImGui::GetIO();

	// 左上に固定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowBgAlpha(0.0f); // 背景を透明にする

	// ウィンドウを作成せずに、文字だけを描画
	ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	// 文字色を緑に変更
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 緑色
	ImGui::Text("FPS: %.1f", io.Framerate);
	ImGui::PopStyleColor();

	ImGui::End();
}

void GUI::ShowDeltaTime() {
	// FPS表示の少し下に固定
	ImGui::SetNextWindowPos(ImVec2(0, 16));
	ImGui::SetNextWindowBgAlpha(0.0f); // 背景を透明にする

	// ウィンドウを作成せずに、文字だけを描画
	ImGui::Begin("DeltaTime Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

	// 文字色を緑に変更
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 緑色
	ImGui::Text("DeltaTime: %.3f", deltaTimer_->GetDeltaTime());
	ImGui::PopStyleColor();

	ImGui::End();
}

void GUI::ShowTextureDatas() {

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	// ウィンドウを開く
	if (ImGui::Begin("Texture List", nullptr, windowFlags)) {
		// テクスチャを管理しているコンテナからマップを取得
		const auto& textureDatas = textureDataContainer_->GetTextureDatas();

		// テクスチャの総数を表示
		ImGui::Text("Total Textures: %d", static_cast<int>(textureDatas.size()));

		// 選択されたテクスチャ名を保存しておく変数（static を使用した例）
		static std::string selectedKey;

		// -----------------------------------------------------
		// (1) ボタン一覧部分を "子ウィンドウ" (スクロール可能) にする
		// -----------------------------------------------------
		//   - 第1引数：子ウィンドウのID（ユニークな文字列）
		//   - 第2引数：サイズ (幅,高さ)；幅を0にすると親ウィンドウ幅に合わせる
		//   - 第3引数：trueにすると枠線を表示（必要なければfalse）
		// -----------------------------------------------------
		ImGui::BeginChild("##TextureListChild", ImVec2(0, 200), true);

		// テクスチャ一覧をボタンで表示
		for (const auto& [key, tex] : textureDatas) {
			if (ImGui::Button(key.c_str())) {
				selectedKey = key;
			}
		}

		// 子ウィンドウの終了
		ImGui::EndChild();

		// 区切り線
		ImGui::Separator();

		// -----------------------------------------------------
		// (2) 選択されたテクスチャの情報表示
		// -----------------------------------------------------
		if (!selectedKey.empty()) {
			// 今選択されているキーに対応するテクスチャを探す
			auto it = textureDatas.find(selectedKey);
			if (it != textureDatas.end()) {
				const auto& selectedTex = it->second;
				const auto& metaData = selectedTex.metaData;

				ImGui::Text("Selected Texture: %s", selectedKey.c_str());
				ImGui::Text("Width     : %zu", metaData.width);
				ImGui::Text("Height    : %zu", metaData.height);
				ImGui::Text("MipLevels : %zu", metaData.mipLevels);
				ImGui::Text("Format    : %u", metaData.format);

				// テクスチャのプレビュー
				D3D12_GPU_DESCRIPTOR_HANDLE handle = srvUavManager_->GetDescriptorHandleGPU(selectedTex.srvIndex);
				ImGui::Image(reinterpret_cast<ImTextureID>(handle.ptr), ImVec2(128.0f, 128.0f));
			} else {
				// 選択キーが見つからなくなった場合
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Selected texture not found!");
			}
		} else {
			ImGui::Text("No texture selected.");
		}
	}
	ImGui::End();
}

void GUI::ShowModelDatas() {

}

void GUI::ShowAnimaionDatas() {

}

void GUI::ShowSoundDatas() {

}

void GUI::ShowColliderManager() {
	// コライダーの一覧を取得
	const auto& colliders = dataIO_->GetColliders();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	// ウィンドウ表示開始（ImGuiウィンドウ）
	ImGui::Begin("Collider Manager", nullptr, windowFlags);

	// コライダー総数を表示
	ImGui::Text("Total Colliders: %d", static_cast<int>(colliders.size()));

	// ファイル名入力用のバッファ (ImGui では文字配列を使うことが多い)
	static char colliderSaveFileName[64] = "DefaultName";

	// セーブ
	ImGui::Text("Save");
	// 同じ行に配置
	ImGui::SameLine();
	// テキスト入力 (ラベルとして "##" から始めるとラベルが非表示になるのでUIがコンパクトになる)
	ImGui::InputText("##SaveFileName", colliderSaveFileName, IM_ARRAYSIZE(colliderSaveFileName));

	// 同じ行に配置
	ImGui::SameLine();

	// 「Save」ボタン
	if (ImGui::Button("Save")) {
		// 入力されたファイル名が空でなければ保存を実行
		if (strlen(colliderSaveFileName) > 0) {
			// データIOクラスを使ってセーブ
			dataIO_->SaveColliderDataFile(colliderSaveFileName);
		} else {
			// 空文字なら何もしない or エラーメッセージ表示
		}
	}


	// 選択中のコライダーを識別するための静的変数
	static int selectedIndex = -1;

	//
	// 左側にコライダー一覧を表示（スクロール可）
	//
	ImGui::BeginChild("ColliderList", ImVec2(200, 100), true);
	for (int i = 0; i < static_cast<int>(colliders.size()); i++) {
		// コライダー名を取得
		const std::string& colliderName = colliders[i]->name_;

		// ボタンとして表示
		// 押されたら選択中のインデックスを更新する
		if (ImGui::Button(colliderName.c_str(), ImVec2(180, 0))) {
			selectedIndex = i;
		}

	}
	ImGui::EndChild();

	// 同じ行に並べたい場合は ImGui::SameLine() を呼び出す
	ImGui::SameLine();

	// コライダーの設定
	ImGui::BeginChild("ColliderSettings", ImVec2(0, 100), true);
	if (selectedIndex >= 0 && selectedIndex < static_cast<int>(colliders.size())) {
		BaseCollider3D* collider = colliders[selectedIndex].get();
		if (collider) {
			//
			// コライダーのタイプを編集 (Combo などで変更)
			//
			int colliderTypeNum = static_cast<int>(collider->GetType());
			// 例として3種類だけ想定
			const char* colliderTypes[] = { "Sphere", "AABB", "OBB" };
			if (ImGui::Combo("Collider Type", &colliderTypeNum, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
				// Comboで選択が変わったらSetType
				collider->GetType() = static_cast<Collider3DType>(colliderTypeNum);
			}

			//
			// オフセットを編集 (DragFloat3など)
			//
			auto offsetPos = collider->GetOffset();
			if (ImGui::DragFloat3("Offset", &offsetPos.x, 0.01f)) {
				// 値が変わったらコライダーに反映
				collider->GetOffset() = offsetPos;
			}

			//
			// 球体コライダーの場合のみ、半径 (radius) を編集
			//
			if (collider->GetType() == Collider3DType::Sphere) {
				// SphereCollider の派生クラスにキャスト
				SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider);
				if (sphere) {
					float radius = sphere->GetRadius();
					if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.0f, 10000.0f)) {
						sphere->GetRadius() = radius;
					}
				}
			}
		}
	}
	ImGui::EndChild();

	// 区切り線
	ImGui::Separator();

	// 有効なインデックスなら、詳細情報を表示
	if (selectedIndex >= 0 && selectedIndex < static_cast<int>(colliders.size())) {
		BaseCollider3D* collider = colliders[selectedIndex].get();
		if (collider) {
			// 所属するゲームオブジェクトの名前
			ImGui::Text("GameObject: %s", collider->GetOwner()->name_.c_str());

			// ワールド座標
			auto worldPos = collider->worldPosition_;
			ImGui::Text("World Position: (%.2f, %.2f, %.2f)", worldPos.x, worldPos.y, worldPos.z);

			// コライダータイプの表示
			Collider3DType cType = collider->GetType();
			switch (cType) {
			case Collider3DType::Sphere:
				ImGui::Text("Collider Type: Sphere");
				// 半径を表示
				{
					SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider);
					if (sphere) {
						ImGui::Text("Radius: %.2f", sphere->GetRadius());
					}
				}
				break;
			case Collider3DType::AABB:
				ImGui::Text("Collider Type: AABB");
				break;
			case Collider3DType::OBB:
				ImGui::Text("Collider Type: OBB");
				break;
			default:
				ImGui::Text("Collider Type: Unknown");
				break;
			}

			// コライダーのオフセット
			auto offsetPos = collider->GetOffset();
			ImGui::Text("Offset: (%.2f, %.2f, %.2f)", offsetPos.x, offsetPos.y, offsetPos.z);

			// アクティブフラグ
			bool isActive = collider->GetIsActive();
			ImGui::Text("Active: %s", isActive ? "true" : "false");
		}
	}

	// ウィンドウを閉じる
	ImGui::End();
}
