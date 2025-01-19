#pragma once

// C++
#include <memory>

#ifdef _DEBUG
#include "LeakChecker/D3DResourceLeakChecker.h"
#endif // _DEBUG

#include "WindowApp/WindowApp.h"
#include "DeltaTimer/DeltaTimer.h"
#include "DirectInput/DirectInput.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/Fence/Fence.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

#include "ViewManagers/RTVManager/RTVManager.h"
#include "ViewManagers/DSVManager/DSVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/ResourceBarrier/ResourceBarrier.h"
#include "DirectX/RenderTarget/RenderTarget.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"

#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"


#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"


#include "SceneManager/SceneManager.h"


#include "ImGuiController/ImGuiController.h"

#include "GUI/GUI.h"

// エンジンの全機能を持つクラス
class MAGISYSTEM {
public:
	// 仮想デストラクタ
	virtual~MAGISYSTEM() = default;
	// 初期化
	virtual void Initialize();
	// 終了
	void Finalize();
	// 更新
	virtual void Update();
	// 描画
	virtual void Draw();

	// 実行
	void Run();

	// 終了チェック
	bool IsEndRequest() const;

public: // エンジンの機能

	// TODO: regionを使わない方法を検討する

#pragma region WindowAppの機能
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	static HWND GetWindowHandle();
#pragma endregion

#pragma region DeltaTimerの機能
	/// <summary>
	/// デルタタイムを取得
	/// </summary>
	/// <returns>デルタタイムを返す</returns>
	static float GetDeltaTime();

#pragma endregion

#pragma region DirectInputの機能
	/// <summary>
	/// 指定番号のキーボードが押されているかどうかを返します
	/// </summary>
	/// <param name="keyNumber">キー番号</param>
	/// <returns>指定したキーが押されているかどうか</returns>
	static bool PushKey(BYTE keyNumber);
	// キーを押した
	static bool TriggerKey(BYTE keyNumber);
	// キーを押し続けている
	static bool HoldKey(BYTE keyNumber);
	// キーを離した
	static bool ReleaseKey(BYTE keyNumber);

	// マウスボタンを押している
	static bool PushMouseButton(MouseButton mouseButton);
	// マウスボタンを押した
	static bool TriggerMouseButton(MouseButton mouseButton);
	// マウスボタンを押し続けている
	static bool HoldMouseButton(MouseButton mouseButton);
	// マウスボタンを離した
	static bool ReleaseMouseButton(MouseButton mouseButton);
	// マウスホイールの入力を取得
	static int64_t GetMouseWheelDelta();
#pragma endregion

#pragma region DXGIの機能
	// デバイス取得
	static ID3D12Device* GetDirectXDevice();
	// バッファリソースを作成
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes, bool isUav = false);
#pragma endregion

#pragma region DirectXCommandの機能
	// コマンドリストを取得
	static ID3D12GraphicsCommandList* GetDirectXCommandList();

#pragma endregion

#pragma region Fenceの機能



#pragma endregion

#pragma region TextureDataContainer



#pragma endregion

private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
protected:
#ifdef _DEBUG
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG

	// 
	// AppBaseSystems
	// 
	static std::unique_ptr<WindowApp> windowApp_;
	static std::unique_ptr<DeltaTimer> deltaTimer_;
	static std::unique_ptr<DirectInput> directInput_;

	// 
	// DirectXBaseSystems
	// 
	static std::unique_ptr<DXGI> dxgi_;
	static std::unique_ptr<DirectXCommand> directXCommand_;
	static std::unique_ptr<Fence> fence_;
	static std::unique_ptr<ShaderCompiler> shaderCompiler_;

	// 
	// ViewManagers
	// 
	static std::unique_ptr<RTVManager> rtvManager_;
	static std::unique_ptr<DSVManager> dsvManager_;
	static std::unique_ptr<SRVUAVManager> srvuavManager_;

	// 
	// DirectXRenderSystems
	// 
	static std::unique_ptr<SwapChain> swapChain_;
	static std::unique_ptr<DepthStencil> depthStencil_;
	static std::unique_ptr<ResourceBarrier> resourceBarrier_;
	static std::unique_ptr<RenderTarget> renderTarget_;
	static std::unique_ptr<Viewport> viewport_;
	static std::unique_ptr<ScissorRect> scissorRect_;

	//
	// PipelineManager
	//
	static std::unique_ptr<GraphicsPipelineManager> graphicsPipelineManager_;

	// 
	// AssetContainer
	// 
	static std::unique_ptr<TextureDataContainer> textureDataCantainer_;

	// 
	// GameManager
	// 
	static std::unique_ptr<SceneManager<GameData>> sceneManager_;

	//
	// UIクラス
	//
	static std::unique_ptr<ImGuiController> imguiController_;
	static std::unique_ptr<GUI> gui_;

};

// シーンで使う一部の機能を持つクラス
class MAGI {
public:

};