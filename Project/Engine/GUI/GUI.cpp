#include "GUI.h"

#include <cassert>

#include "imgui/imgui.h"

#include "DeltaTimer/DeltaTimer.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"
#include "DataIO/DataIO.h"
#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"

#include "Renderer3DManager/Renderer3DManager.h"
#include "ColliderManager/ColliderManager.h"

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

	//
	// アセットコンテナの描画
	//

	// テクスチャ
	ShowTextureDatas();


	// 
	// オブジェクトマネージャの描画
	// 

	// 3D描画オブジェクト
	ShowRenderer3DManager();

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

void GUI::ShowRenderer3DManager() {
	// 3D描画オブジェクトの一覧を取得
	const auto& renderers = renderer3DManager_->GetRenderers();

	// 選択中の描画オブジェクトを識別するための静的変数
	static int selectedRenderer3DIndex = -1;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	// ウィンドウ表示開始（ImGuiウィンドウ）
	ImGui::Begin("3DRenderer Manager", nullptr, windowFlags);
	// 総数を表示
	ImGui::Text("Total Renderers3D: %d", static_cast<int>(renderers.size()));

	// 描画オブジェクトデータセーブ関数
	ShowRenderer3DDataSaveUI();
	// 描画オブジェクトデータロード関数
	ShowRenderer3DDataLoadUI();

	// 描画オブジェクト作成関数
	ShowCreateRenderer3DUI();

	// 描画オブジェクトリスト描画関数
	ShowRenderer3DList(renderers, selectedRenderer3DIndex);

	// 描画オブジェクト設定描画
	ShowRenderer3DSetting(renderers, selectedRenderer3DIndex);

	// 区切り線
	ImGui::Separator();

	// 描画オブジェクト情報表示関数
	ShowRenderer3DInformation(renderers, selectedRenderer3DIndex);

	// ウィンドウを閉じる
	ImGui::End();
}

void GUI::ShowRenderer3DDataSaveUI() {
	// ファイル名入力用のバッファ
	static char renderers3DSaveFileName[64] = "Renderers3DData.json";

	// セーブ
	ImGui::Text("Save");
	// 同じ行に配置
	ImGui::SameLine();
	// テキスト入力
	ImGui::InputText("##Renderers3DSaveFileName", renderers3DSaveFileName, IM_ARRAYSIZE(renderers3DSaveFileName));

	// 同じ行に配置
	ImGui::SameLine();

	// 「Save」ボタン
	if (ImGui::Button("Save")) {
		// 入力されたファイル名が空でなければ保存を実行
		if (strlen(renderers3DSaveFileName) > 0) {
			// データIOクラスを使ってセーブ
			dataIO_->SaveRenderer3DDataFile(renderers3DSaveFileName);
		} else {
			// 空文字なら何もしない or エラーメッセージ表示
		}
	}
}

void GUI::ShowRenderer3DDataLoadUI() {
	// ファイル名入力用のバッファ
	static char renderers3DLoadFileName[64] = "Renderers3DData.json";

	// セーブ
	ImGui::Text("Load");
	// 同じ行に配置
	ImGui::SameLine();
	// テキスト入力
	ImGui::InputText("##Renderers3DLoadFileName", renderers3DLoadFileName, IM_ARRAYSIZE(renderers3DLoadFileName));

	// 同じ行に配置
	ImGui::SameLine();

	// 「Save」ボタン
	if (ImGui::Button("Load")) {
		// 入力されたファイル名が空でなければ保存を実行
		if (strlen(renderers3DLoadFileName) > 0) {
			// データIOクラスを使ってセーブ
			dataIO_->LoadRenderer3DDataFile(renderers3DLoadFileName);
		} else {
			// 空文字なら何もしない or エラーメッセージ表示
		}
	}
}

void GUI::ShowCreateRenderer3DUI() {

}

void GUI::ShowCreateRenderer3DWindow() {

}

void GUI::ShowRenderer3DList(const std::vector<std::unique_ptr<BaseRenderable3D>>& renderers3D, int& selectedIndex) {
	//
	// 左側に一覧を表示（スクロール可）
	//
	ImGui::Text("3DRendererList");
	ImGui::BeginChild("Renderer3DList", ImVec2(200, 100), true);
	for (int i = 0; i < static_cast<int>(renderers3D.size()); i++) {
		// 名前を取得
		const std::string& rendererName = renderers3D[i]->name_;

		// ボタンとして表示
		// 押されたら選択中のインデックスを更新する
		if (ImGui::Button(rendererName.c_str(), ImVec2(180, 0))) {
			selectedIndex = i;
		}

	}
	ImGui::EndChild();
}

void GUI::ShowRenderer3DSetting(const std::vector<std::unique_ptr<BaseRenderable3D>>& renderers, int& selectedIndex) {
	// 描画オブジェクトの設定
	renderers;
	selectedIndex;
}

void GUI::ShowRenderer3DInformation(const std::vector<std::unique_ptr<BaseRenderable3D>>& renderers, int& selectedIndex) {
	// 描画オブジェクトの情報
	renderers;
	selectedIndex;
}

void GUI::ShowColliderManager() {
	// コライダーの一覧を取得
	const auto& colliders = colliderManager_->GetColliders();

	// 選択中のコライダーを識別するための静的変数
	static int selectedColliderIndex = -1;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	// ウィンドウ表示開始（ImGuiウィンドウ）
	ImGui::Begin("Collider Manager", nullptr, windowFlags);
	// 総数を表示
	ImGui::Text("Total Colliders: %d", static_cast<int>(colliders.size()));

	// コライダーセーブ関数
	ShowColliderDataSaveUI();
	// コライダーロード関数
	ShowColliderDataLoadUI();

	// コライダー作成UI管理関数
	ShowCreateColliderUI();

	// コライダーリスト表示関数
	ShowColliderList(colliders, selectedColliderIndex);

	// 同じ行に並べたい場合は ImGui::SameLine() を呼び出す
	ImGui::SameLine();

	// コライダーの設定関数
	ShowColliderSetting(colliders, selectedColliderIndex);

	// 区切り線
	ImGui::Separator();

	// コライダーの情報表示関数
	ShowColliderInformation(colliders, selectedColliderIndex);

	// ウィンドウを閉じる
	ImGui::End();
}

void GUI::ShowColliderDataSaveUI() {
	// ファイル名入力用のバッファ
	static char colliderSaveFileName[64] = "CollidersData.json";

	// セーブ
	ImGui::Text("Save");
	// 同じ行に配置
	ImGui::SameLine();
	// テキスト入力
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
}

void GUI::ShowColliderDataLoadUI() {
	// ファイル名入力用のバッファ
	static char colliderLoadFileName[64] = "CollidersData.json";

	// セーブ
	ImGui::Text("Load");
	// 同じ行に配置
	ImGui::SameLine();
	// テキスト入力
	ImGui::InputText("##LoadFileName", colliderLoadFileName, IM_ARRAYSIZE(colliderLoadFileName));

	// 同じ行に配置
	ImGui::SameLine();

	// 「Save」ボタン
	if (ImGui::Button("Load")) {
		// 入力されたファイル名が空でなければ保存を実行
		if (strlen(colliderLoadFileName) > 0) {
			// データIOクラスを使ってセーブ
			dataIO_->LoadColliderDataFile(colliderLoadFileName);
		} else {
			// 空文字なら何もしない or エラーメッセージ表示
		}
	}
}

void GUI::ShowCreateColliderUI() {
	// コライダー作成ボタン
	if (ImGui::Button("Create New Collider")) {
		showColliderCreateWindow_ = true;
	}
	// コライダー作成ウィンドウ
	if (showColliderCreateWindow_) {
		// 第2引数に &showCreateWindow を渡すことで、×ボタンを押すと false になる
		ImGui::Begin("Create Collider", &showColliderCreateWindow_, ImGuiWindowFlags_AlwaysAutoResize);

		// 新規作成用のUIを表示
		ShowCreateColliderWindow();

		ImGui::End();
	}
}

void GUI::ShowCreateColliderWindow() {
	static char colliderName[128] = "NewCollider";  // 初期値適当
	static int selectedTypeIndex = 0;
	static CollisionCategory selectedCategory = CollisionCategory::None; // 列挙型で管理
	static float offset[3] = { 0.0f, 0.0f, 0.0f };
	static float radius = 1.0f; // Sphere用

	// ウィンドウが初めて表示されたタイミングで初期状態にリセット
	if (ImGui::IsWindowAppearing()) {
		strcpy_s(colliderName, "NewCollider");  // 初期値にリセット
		selectedTypeIndex = 0;
		selectedCategory = CollisionCategory::None;
		offset[0] = 0.0f; offset[1] = 0.0f; offset[2] = 0.0f;
		radius = 1.0f;
	}

	// コライダー名の入力
	ImGui::InputText("Collider Name", colliderName, IM_ARRAYSIZE(colliderName));

	// コライダータイプの選択
	const char* colliderTypes[] = { "Sphere", "AABB", "OBB" };
	ImGui::Combo("Collider Type", &selectedTypeIndex, colliderTypes, IM_ARRAYSIZE(colliderTypes));

	// コライダーカテゴリーの選択
	int selectedCategoryIndex = static_cast<int>(selectedCategory);
	const std::vector<const char*>& categoryList = GetCollisionCategoryList();
	if (ImGui::Combo("Collider Category", &selectedCategoryIndex, categoryList.data(), static_cast<int>(categoryList.size()))) {
		selectedCategory = static_cast<CollisionCategory>(selectedCategoryIndex);
	}

	// Offset
	ImGui::InputFloat3("Offset", offset);

	// 球体コライダーの場合
	if (selectedTypeIndex == 0) {
		ImGui::InputFloat("Radius", &radius);
	}

	// 作成ボタン
	if (ImGui::Button("Create")) {
		// コライダーを作成して取得
		BaseCollider3D* newCollider = colliderManager_->Find(colliderManager_->Create(colliderName, static_cast<Collider3DType>(selectedTypeIndex)));
		if (newCollider) {
			newCollider->GetOffset() = { offset[0], offset[1], offset[2] };
			newCollider->GetCategory() = selectedCategory; // カテゴリを設定

			// 球体コライダーの場合は半径をセット
			if (selectedTypeIndex == 0) {
				if (auto sphereCollider = dynamic_cast<SphereCollider*>(newCollider)) {
					sphereCollider->GetRadius() = radius;
				}
			}
		}

		// ウィンドウを非表示に
		showColliderCreateWindow_ = false;
	}
}

void GUI::ShowColliderList(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex) {
	//
	// 左側にコライダー一覧を表示（スクロール可）
	//
	ImGui::Text("ColliderList");
	ImGui::BeginChild("ColliderList", ImVec2(200, 130), true);
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
}

void GUI::ShowColliderSetting(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex) {
	// コライダーの設定
	ImGui::BeginChild("ColliderSettings", ImVec2(0, 130), true);
	if (selectedIndex >= 0 && selectedIndex < static_cast<int>(colliders.size())) {
		BaseCollider3D* collider = colliders[selectedIndex].get();
		if (collider) {

			//
			// 有効フラグを編集
			//
			{
				auto isActive = collider->GetIsActive();
				if (ImGui::Checkbox("IsActive", &isActive)) {
					collider->GetIsActive() = isActive;
				}
			}

			// 
			// カテゴリを編集
			// 
			{
				CollisionCategory selectedCategory = collider->GetCategory();
				int selectedCategoryIndex = static_cast<int>(selectedCategory);
				const std::vector<const char*>& categoryList = GetCollisionCategoryList();
				if (ImGui::Combo("Collider Category", &selectedCategoryIndex, categoryList.data(), static_cast<int>(categoryList.size()))) {
					selectedCategory = static_cast<CollisionCategory>(selectedCategoryIndex);
					collider->GetCategory() = selectedCategory;
				}
			}

			//
			// コライダータイプの編集
			//
			{
				Collider3DType currentType = collider->GetType();
				int selectedTypeIndex = static_cast<int>(currentType);
				const char* colliderTypes[] = { "Sphere", "AABB", "OBB" };

				if (ImGui::Combo("Collider Type", &selectedTypeIndex, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
					// 変更されたら、新しいコライダーを作成
					std::string colliderName = collider->name_;
					CollisionCategory category = collider->GetCategory(); // カテゴリの保持
					auto offset = collider->GetOffset(); // オフセットを保持
					bool isActive = collider->GetIsActive(); // 有効フラグ保持

					// 古いコライダーを削除
					colliderManager_->Remove(colliderName);

					// 新しいコライダーを作成
					colliderManager_->Create(colliderName, static_cast<Collider3DType>(selectedTypeIndex));
					BaseCollider3D* newCollider = colliderManager_->Find(colliderName);
					if (newCollider) {
						newCollider->GetOffset() = offset;
						newCollider->GetIsActive() = isActive;
						newCollider->GetCategory() = category;
					}

					// 更新
					collider = newCollider;
				}
			}

			//
			// オフセットを編集
			//
			{
				auto offsetPos = collider->GetOffset();
				if (ImGui::DragFloat3("Offset", &offsetPos.x, 0.01f)) {
					collider->GetOffset() = offsetPos;
				}
			}

			//
			// 球体コライダーの場合のみ、半径 (radius) を編集
			//
			{
				if (collider->GetType() == Collider3DType::Sphere) {
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
	}
	ImGui::EndChild();
}

void GUI::ShowColliderInformation(const std::vector<std::unique_ptr<BaseCollider3D>>& colliders, int& selectedIndex) {
	// 有効なインデックスなら、詳細情報を表示
	if (selectedIndex >= 0 && selectedIndex < static_cast<int>(colliders.size())) {
		BaseCollider3D* collider = colliders[selectedIndex].get();
		if (collider) {
			if (collider->GetOwner()) {
				// ゲームオブジェクトに所属している場合
				ImGui::Text("GameObject: %s", collider->GetOwner()->name_.c_str());
			} else {
				// 所属するゲームオブジェクトがない場合
				ImGui::Text("GameObject: Not Owner");
			}

			// アクティブフラグ
			bool isActive = collider->GetIsActive();
			ImGui::Text("Active: %s", isActive ? "true" : "false");

			// コライダーのカテゴリ表示
			CollisionCategory category = collider->GetCategory();
			std::string categoryStr = CollisionCategoryToString(category);
			ImGui::Text("Category: %s", categoryStr.c_str());

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

			// ワールド座標
			auto worldPos = collider->worldPosition_;
			ImGui::Text("World Position: (%.2f, %.2f, %.2f)", worldPos.x, worldPos.y, worldPos.z);

		}
	}
}
