#pragma once

// C++
#include <memory>

#ifdef _DEBUG
#include "LeakChecker/D3DResourceLeakChecker.h"
#endif // _DEBUG

#include "WindowApp/WindowApp.h"

// エンジンの全機能を持つ
class MAGISYSTEM {
public:
	// 仮想デストラクタ
	virtual~MAGISYSTEM() = default;
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 終了チェック
	bool IsEndRequest() const;
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
private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
private:
#ifdef _DEBUG
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG
	// WindowApp
	static std::unique_ptr<WindowApp> windowApp_;

};

// シーンで使う一部の機能を持つ
class MAGI {
public:

};