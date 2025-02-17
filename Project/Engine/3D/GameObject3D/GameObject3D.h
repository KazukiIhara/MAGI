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
	virtual void Draw();

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
	virtual void OnCollisionEnter([[maybe_unused]] GameObject3D* other);
	// 衝突中
	virtual void OnCollisionStay([[maybe_unused]] GameObject3D* other);
	// 離れた
	virtual void OnCollisionExit([[maybe_unused]] GameObject3D* other);

	//
	// コライダーの設定関数
	//

	// アクティブフラグの設定
	bool& GetColliderIsActive(const std::string& name);

	// オフセットの取得
	Vector3& GetColliderOffset(const std::string& name);

	//
	// スフィアコライダー用の設定
	// 
	float& GetColliderRadius(const std::string& name);

	//
	// コンポーネント追加関数
	//

	// シンプル形状描画オブジェクト生成
	void CreatePrimitiveRenderer(const std::string& rendererName, const Primitive3DType& primitiveType, const std::string& textureName = "");
	// スキニングなしモデル描画オブジェクト生成
	void CreateStaticRenderer(const std::string& rendererName, const std::string& modelName);
	// スキニングありモデル描画オブジェクト生成
	void CreateSkinningRenderer(const std::string& rendererName, const std::string& modelName);

	// コライダーの追加関数
	void AddCollider(BaseCollider3D* collider);

private:
	// ワールドトランスフォーム作成
	void CreateWorldTransform(const EulerTransform3D& transform);
private:
	// ワールド上の姿勢
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
	// 描画用クラス
	std::unique_ptr<BaseRenderable3D> renderer3D_ = nullptr;
	// コライダーを受け取る箱
	std::map<std::string, BaseCollider3D*> colliders3D_;
};