#include "GUI.h"

#include <cassert>

#include "imgui/imgui.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "DataIO/DataIO.h"
#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"

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
	// ウィンドウを開く
	if (ImGui::Begin("Texture List")) {
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
