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
	// シェーダをコンパイル
	ComPtr<ID3DBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);
private:
	// 初期化
	void Initialize();
private:
	// シェーダーコンパイルに使用するオブジェクト
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
};