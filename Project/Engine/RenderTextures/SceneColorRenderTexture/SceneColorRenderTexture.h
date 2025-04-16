#pragma once

#include "RenderTextures/BaseRenderTexture/BaseRenderTexture.h"


class SceneColorRenderTexture : public BaseRenderTexture {
public:
	SceneColorRenderTexture();
	~SceneColorRenderTexture()override;

	void Initialize();

private:

};