#pragma once

// C++
#include <string>
#include <unordered_map>

// 前方宣言
class Transform3D;

/// <summary>
/// シーン上のオブジェクト
/// </summary>
class GameObject3D {
public:
	GameObject3D(const std::string& name);
	~GameObject3D();

	// 更新
	void Update();


	// トランスフォームのポインタを取得
	Transform3D* GetTransform();

private:
	// 名前
	std::string name_ = "";

	//=======================
	// コンポーネント
	//=======================

	// トランスフォーム
	Transform3D* transformComponent_ = nullptr;
	// 描画コンポーネント

};