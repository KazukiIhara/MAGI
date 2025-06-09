#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>

// 前方宣言
class Transform3D;
class ModelRenderer;

/// <summary>
/// シーン上のオブジェクト
/// </summary>
class GameObject3D {
public:
	GameObject3D(const std::string& name);
	~GameObject3D();

	// 更新
	void Update();

	void Finalize();

	void AddModelRenderer(std::unique_ptr<ModelRenderer> modelRenderer);

	void SetIsAlive(bool isAlive);
	void SetIsActive(bool isActive);

	[[nodiscard]] const bool& GetIsAlive()const;
	[[nodiscard]] const bool& GetIsActive()const;
	[[nodiscard]] Transform3D* GetTransform();

private:
	// 名前
	std::string name_ = "";
	// 生存フラグ
	bool isAlive_ = true;
	// 有効フラグ
	bool isActive_ = true;

	//=======================
	// コンポーネント
	//=======================

	// トランスフォーム
	Transform3D* transformComponent_ = nullptr;
	// モデル描画コンポーネント
	std::unordered_map<std::string, ModelRenderer*> modelRendererComponents_;

};