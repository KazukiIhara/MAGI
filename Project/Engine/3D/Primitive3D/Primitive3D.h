#pragma once

// C++
#include <memory>

#include "3D/BaseRenderable3D/BaseRenderable3D.h"

class Primitive3D :public BaseRenderable3D {
public:
	Primitive3D(const std::string& objectName);
	~Primitive3D()override;
	void Initialize(const std::string& objectName);
	void Update()override;
	void Draw()override;
private:

};