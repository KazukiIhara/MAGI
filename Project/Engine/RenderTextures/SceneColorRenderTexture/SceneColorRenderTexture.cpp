#include "SceneColorRenderTexture.h"

SceneColorRenderTexture::SceneColorRenderTexture() {
}

SceneColorRenderTexture::~SceneColorRenderTexture() {
}

void SceneColorRenderTexture::Initialize() {
	// シーンカラー描画用の標準フォーマットで初期化
	BaseRenderTexture::Create(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		Vector4(0.3f, 0.3f, 0.3f, 1.0f)
	);
}
