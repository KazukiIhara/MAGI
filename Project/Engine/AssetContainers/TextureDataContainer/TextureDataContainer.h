#pragma once

// C++
#include <unordered_map>
#include <string>

#include "Structs/TextureStruct.h"

// 前方宣言
class DXGI;
class DirectXCommand;
class Fence;
class SRVUAVManager;

/// <summary>
/// テクスチャデータのコンテナ
/// </summary>
class TextureDataContainer {
public:
	TextureDataContainer(DXGI* dxgi, DirectXCommand* command, Fence* fence, SRVUAVManager* srvUavManagerManager);
	~TextureDataContainer();

	// 初期化
	void Initialize(DXGI* dxgi, DirectXCommand* command, Fence* fence, SRVUAVManager* srvUavManager);
	// テクスチャのロード
	void Load(const std::string& filePath);
	// Textureを渡す
	std::unordered_map<std::string, Texture>& GetTexture();
	// メタデータを渡す
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);
private:
	// Texture読み込み
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	// テクスチャリソースを作る
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	// テクスチャデータを送る
	ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
private:
	// DXGIのインスタンスをセット
	void SetDXGI(DXGI* dxgi);
	// Commandのインスタンスをセット
	void SetDirectXCommand(DirectXCommand* directXCommand);
	// Fenceのインスタンスをセット
	void SetFence(Fence* fence);
	// SrvUavManager
	void SetSrvUavManager(SRVUAVManager* srvUavManager);
private:
	// テクスチャデータコンテナ
	std::unordered_map<std::string, Texture> textureDatas_;
private:
	// DXGI
	DXGI* dxgi_ = nullptr;
	// DirectXCommand
	DirectXCommand* directXCommand_ = nullptr;
	// Fence
	Fence* fence_ = nullptr;
	// SrvManager
	SRVUAVManager* srvUavManager_ = nullptr;
};