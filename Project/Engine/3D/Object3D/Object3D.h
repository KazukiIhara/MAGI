#pragma once

#include "3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/Model/Model.h"

/// <summary>
/// 3Dオブジェクト(インスタンスの生成後初期化メソッドを呼んでください)
/// </summary>
class Object3D :public BaseRenderable3D {
public:
	Object3D(const std::string& objectName, const std::string& modelName);
	virtual ~Object3D()override;
	virtual void Initialize();
	virtual void Update()override;
	void Draw()override;

protected:
	// 受け取るモデルのデータ
	ModelData modelData_{};
	// モデル
	std::unique_ptr<Model> model_ = nullptr;

};