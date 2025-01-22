#pragma once

// C++
#include <memory>

#include "3D/BaseRenderable3D/BaseRenderable3D.h"
#include "3D/Model/Model.h"

class Object3D :public BaseRenderable3D {
public:
	Object3D(const std::string& objectName, const std::string& modelName);
	~Object3D()override;
	void Initialize(const std::string& modelName);
	void Update()override;
	void Draw()override;
private:
	// モデル
	std::unique_ptr<Model> model_ = nullptr;
};