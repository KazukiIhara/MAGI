#include "GUI.h"

#include <cassert>

#include "imgui/imgui.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "DataIO/DataIO.h"
#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"

#include "ObjectManagers/Renderer3DManager/Renderer3DManager.h"
#include "ObjectManagers/ColliderManager/ColliderManager.h"

#include "Logger/Logger.h"

#include "3D/GameObject3D/GameObject3D.h"

// Windows環境限定
#ifdef _WIN32
#include <Windows.h>
#endif

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


	// 
	// データIOクラスから受け取るインスタンス
	// 

	renderer3DManager_ = dataIO_->GetRenderer3DManager();
	assert(renderer3DManager_);

	colliderManager_ = dataIO_->GetColliderManager();
	assert(colliderManager_);

}

void GUI::Update() {
	//
	// デバッグ描画
	//
	ShowFPS();
	ShowDeltaTime();

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