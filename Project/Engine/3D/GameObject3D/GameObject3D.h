#pragma once

#include <memory>
#include <vector>

#include "Structs/ObjectStruct.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"
#include "3D/RenderObjects3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/RenderObjects3D/PrimitiveRenderer3D/PrimitiveRenderer3D.h"
#include "3D/RenderObjects3D/StaticRenderer3D/StaticRenderer3D.h"
#include "3D/RenderObjects3D/SkinningRenderer3D/SkinningRenderer3D.h"

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"
#include "3D/Colliders3D/SphereCollider/SphereCollider.h"

/// <summary>
/// 3Dゲームオブジェクトクラス
/// </summary>
class GameObject3D :public WorldEntity {
public:
	GameObject3D(const std::string& objectName, const EulerTransform3D& transform = EulerTransform3D{});
	~GameObject3D()override = default;

	virtual void Initialize();
	virtual void Update();

	void UpdateWorldTransform();
	void Draw();

	//
	// トランスフォーム取得関数
	//

	// スケール
	Vector3& GetScale();
	// 回転
	Vector3& GetRotate();
	// 移動量
	Vector3& GetTranslate();

	// ワールドトランスフォームの取得
	WorldTransform* GetWorldTransform();

	// 
	// 衝突応答関数
	// 

	// 衝突した
	virtual void OnCollisionEnter([[maybe_unused]] BaseCollider3D* other);
	// 衝突中
	virtual void OnCollisionStay([[maybe_unused]] BaseCollider3D* other);
	// 離れた
	virtual void OnCollisionExit([[maybe_unused]] BaseCollider3D* other);

	// 
	// コンポーネントのポインタを取得
	// 

	// 3D描画用オブジェクト
	BaseRenderable3D* GetRenderer3D(const std::string& name);

	// 3Dコライダー
	BaseCollider3D* GetCollider3D(const std::string& name);

	//
	// コンポーネント追加関数
	//

	// 描画オブジェクト追加関数
	void AddRenderer3D(BaseRenderable3D* renderer3D);

	// コライダーの追加関数
	void AddCollider(BaseCollider3D* collider3D);

private:
	// ワールドトランスフォーム作成
	void CreateWorldTransform(const EulerTransform3D& transform);
private:
	// ワールド上の姿勢
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// 3D描画用オブジェクトを受け取る箱
	std::map<std::string, BaseRenderable3D*> renderers3D_;
	// コライダーを受け取る箱
	std::map<std::string, BaseCollider3D*> colliders3D_;
};