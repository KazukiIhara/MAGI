#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>
#include <dxcapi.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"

class ShaderCompiler {
public:
	ShaderCompiler();
	~ShaderCompiler();

	// 初期化
	void Initialize();

	// シェーダをコンパイル
	ComPtr<ID3DBlob> CompileShader(
		// シェーダーファイルのパス
		const std::wstring& filePath,
		// コンパイルに使用するプロファイル
		const wchar_t* profile,
		// 初期化で生成したシェーダーコンパイラ関連の3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

private:
	// シェーダーコンパイルに使用するオブジェクト
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
};