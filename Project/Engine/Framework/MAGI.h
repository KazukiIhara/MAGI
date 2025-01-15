#pragma once

// C++
#include <memory>

#ifdef _DEBUG
#include "LeakChecker/D3DResourceLeakChecker.h"
#endif // _DEBUG

#include "WindowApp/WindowApp.h"
#include "DirectInput/DirectInput.h"

#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/Fence/Fence.h"

#include "ViewManagers/RTVManager/RTVManager.h"
#include "ViewManagers/DSVManager/DSVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"


// エンジンの全機能を持つクラス
class MAGISYSTEM {
public:
	// 仮想デストラクタ
	virtual~MAGISYSTEM() = default;
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	virtual void Update();
	// 描画
	virtual void Draw();
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
	// 実行
	void Run();

	// 終了チェック
	bool IsEndRequest() const;

public: // エンジンの機能

	// TODO: 関数の説明をGetWindowHandleを参考にきれいに直す
	// TODO: regionを使わない方法を検討する

#pragma region WindowAppの機能
	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	static HWND GetWindowHandle();
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



private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
private:
#ifdef _DEBUG
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG

	// 
	// WindowAppBaseSystems
	// 
	static std::unique_ptr<WindowApp> windowApp_;
	static std::unique_ptr<DirectInput> directInput_;

	// 
	// DirectXBaseSystems
	// 
	static std::unique_ptr<DXGI> dxgi_;
	static std::unique_ptr<DirectXCommand> directXCommand_;
	static std::unique_ptr<Fence> fence_;

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

};

// シーンで使う一部の機能を持つクラス
class MAGI {
public:

};