#include "ShaderCompiler.h"

#include <cassert>

#include "Logger/Logger.h"

ShaderCompiler::ShaderCompiler() {
	Initialize();
	Logger::Log("ShaderCompiler Initialize");
}

ShaderCompiler::~ShaderCompiler() {
	Logger::Log("ShaderCompiler Finalize");
}

ComPtr<ID3DBlob> ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile) {
	// これからシェーダーをコンパイルする旨をログに出す
	Logger::Log(Logger::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知

	LPCWSTR arguments[] = {
		filePath.c_str(),// コンパイル対象のhlslファイル名
		L"-E",L"main",// エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,// ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",// デバッグ用の情報を埋め込む
		L"-Od",// 最適化を外しておく
		L"-Zpr",// メモリレイアウトは行優先
	};
	// 実際にシェーダーをコンパイルする
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler_,			// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)// コンパイル結果
	);
	// コンパイルエラーではなく、dxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	// 警告、エラーが出てたらログに出して止める
	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Logger::Log(shaderError->GetStringPointer());
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Logger::Log(Logger::ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));

	// 実行用のバイナリを返す
	return shaderBlob;
}

void ShaderCompiler::Initialize() {
	HRESULT hr = S_FALSE;
	// dxCompilerを初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}