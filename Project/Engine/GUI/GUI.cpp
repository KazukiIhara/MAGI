#include "GUI.h"

#include <cassert>

#include "imgui/imgui.h"
#include "Logger/Logger.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "DataIO/DataIO.h"
#include "OffScreenRenderer/OffScreenRenderer.h"

// Windows環境限定
#ifdef _WIN32
#include <Windows.h>
#endif
GUI::GUI(DeltaTimer* deltaTimer, SRVUAVManager* srvUavManager, DataIO* dataIO, OffScreenRenderer* offScreenRenderer) {
	// 
	// インスタンスのセット
	// 
	assert(deltaTimer);
	assert(srvUavManager);
	assert(dataIO);
	assert(offScreenRenderer);
	deltaTimer_ = deltaTimer;
	srvUavManager_ = srvUavManager;
	dataIO_ = dataIO;
	offScreenRenderer_ = offScreenRenderer;

	// 
	// データIOクラスから受け取るインスタンス
	// 


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

void GUI::ShowMainUI() {
	ImGui::Begin("Scene");

	// レンダーテクスチャに描画する内容をウィンドウ内に描画する
	uint32_t srvIndex = offScreenRenderer_->GetCurrentRenderTextureSrvIndex();
	ImGui::ImageWithBg(static_cast<ImTextureID>(srvUavManager_->GetDescriptorHandleGPU(srvIndex).ptr), ImVec2(1280.0f, 720.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui::End();
}

bool& GUI::GetIsShowEngineWindow() {
	return isShowEngineWindow_;
}