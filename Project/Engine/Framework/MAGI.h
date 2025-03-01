#pragma once

// C++
#include <memory>

#ifdef _DEBUG
#include "LeakChecker/D3DResourceLeakChecker.h"
#endif // _DEBUG

// 
// BaseSystems
// 
#include "WindowApp/WindowApp.h"
#include "DeltaTimer/DeltaTimer.h"    
#include "DirectInput/DirectInput.h"

// 
// DirectXBaseSystems
// 
#include "DirectX/DXGI/DXGI.h"
#include "DirectX/DirectXCommand/DirectXCommand.h"
#include "DirectX/Fence/Fence.h"
#include "DirectX/ShaderCompiler/ShaderCompiler.h"

// 
// ViewManagers
// 
#include "ViewManagers/RTVManager/RTVManager.h"
#include "ViewManagers/DSVManager/DSVManager.h"
#include "ViewManagers/SRVUAVManager/SRVUAVManager.h"

// 
// DirectXRenderSystems
// 
#include "DirectX/SwapChain/SwapChain.h"
#include "DirectX/DepthStencil/DepthStencil.h"
#include "DirectX/ResourceBarrier/ResourceBarrier.h"
#include "DirectX/RenderTarget/RenderTarget.h"
#include "DirectX/Viewport/Viewport.h"
#include "DirectX/ScissorRect/ScissorRect.h"

// 
// AssetContainer
// 
#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"
#include "AssetContainers/PrimitiveShapeDataContainer/PrimitiveShapeDataContainer.h"
#include "AssetContainers/ModelDataContainer/ModelDataContainer.h"
#include "AssetContainers/AnimationDataContainer/AnimationDataContainer.h"
#include "AssetContainers/SoundDataContainer/SoundDataContainer.h"

// 
// PipelineManager
// 
#include "PipelineManagers/GraphicsPipelineManager/GraphicsPipelineManager.h"
#include "PipelineManagers/ComputePipelineManager/ComputePipelineManager.h"

// 
// ObjectManager
// 
#include "ObjectManagers/GameObject3DManager/GameObject3DManager.h"
#include "ObjectManagers/Camera3DManager/Camera3DManager.h"
#include "ObjectManagers/PunctualLightManager/PunctualLightManager.h"
#include "ObjectManagers/Renderer3DManager/Renderer3DManager.h"
#include "ObjectManagers/ColliderManager/ColliderManager.h"
#include "ObjectManagers/Emitter3DManager/Emitter3DManager.h"
#include "ObjectManagers/ParticleGroup3DManager/ParticleGroup3DManager.h"

// 
// Drawer
// 
#include "LineDrawer3D/LineDrawer3D.h"

// 
// GameManager
// 
#include "CollisionManager/CollisionManager.h"
#include "SceneManager/SceneManager.h"

// 
// Data入出力クラス
// 
#include "DataIO/DataIO.h"

// 
// UIクラス
// 
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
	// コマンドのクローズと実行
	static void KickCommand();
	// コマンドのリセット
	static void ResetCommand();
#pragma endregion

#pragma region Fenceの機能
	// GPUを待機
	static void WaitGPU();
#pragma endregion

#pragma region SRVUAVManagerの機能
	// SRVUAVのディスクリプタヒープを取得
	static ID3D12DescriptorHeap* GetSrvUavDescriptorHeap();

	// SRVのCPUディスクリプタハンドルを取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHandleCPU(uint32_t index);
	// SRVのGPUディスクリプタハンドルを取得
	static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvUavDescriptorHandleGPU(uint32_t index);
	// Allocate
	static uint32_t ViewAllocate();
	// StructuredBuffer用のsrv作成
	static void CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
	// StructuredBuffer用のUAV作成
	static void CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride);
#pragma endregion

#pragma region GraphicsPipelineManager
	// パイプライン取得関数
	static ID3D12PipelineState* GetGraphicsPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode);

#pragma endregion

#pragma region ComputePipelineManager
	// ルートシグネイチャ取得関数
	static ID3D12RootSignature* GetComputeRootSignature(ComputePipelineStateType pipelineState);
	// パイプライン取得関数
	static ID3D12PipelineState* GetComputePipelineState(ComputePipelineStateType pipelineState);
#pragma endregion

#pragma region TextureDataContainer
	// 画像読み込み
	static void LoadTexture(const std::string& filePath, bool isFullPath = false);
	// 法線マップ画像の読み込み
	static void LoadNormalMapTexture(const std::string& filePath);
	// テクスチャの取得
	static std::map<std::string, Texture>& GetTexture();
	// メタデータ取得
	static const DirectX::TexMetadata& GetTextureMetaData(const std::string& filePath);
#pragma endregion

#pragma region PrimitiveShapeDataContainer
	// 形状の取得
	static PrimitiveData GetPrimitiveShape(const Primitive3DType& primitive3dType);
#pragma endregion

#pragma region ModelDataContainer
	// モデルの読み込み
	static void LoadModel(const std::string& modelName);
	// 読み込み済みモデル検索
	static ModelData FindModel(const std::string& modelName);
#pragma endregion

#pragma region AnimationDataContainer
	// アニメーションの読み込み
	static void LoadAnimation(const std::string& animationFileName, bool isInSameDirectoryAsModel = true);
	// 読み込み済みアニメーションの検索
	static AnimationData FindAnimation(const std::string& animationName);
#pragma endregion

#pragma region SoundDataContainer
	// サウンドの読み込み
	static void LoadWaveSound(const std::string& fileName);
	// サウンドの再生
	static void PlayWaveSound(const std::string& fileName);
	// サウンドループ再生
	static void PlayLoopWaveSound(const std::string& fileName);
	// サウンドの停止
	static void StopWaveSound(const std::string& fileName);
	// サウンドループ再生停止
	static void StopLoopWaveSound(const std::string& fileName);

#pragma endregion

#pragma region GameObject3DManager
	// 3Dゲームオブジェクトの追加
	static void AddGameObject3D(std::unique_ptr<GameObject3D> newGameObject3D);
	// 3Dゲームオブジェクトの削除
	static void RemoveGameObject3D(const std::string& objectName);
	// 3Dゲームオブジェクトを取得
	static GameObject3D* FindGameObject3D(const std::string& objectName);
#pragma endregion

#pragma region Camera3DManager
	// カメラの追加
	static void AddCamera3D(std::unique_ptr<Camera3D> newCamera3D);
	// カメラの削除
	static void RemoveCamera3D(const std::string& cameraName);
	// カメラの取得
	static Camera3D* FindCamera3D(const std::string& cameraName);
	// 使用するカメラのセット
	static void SetCurrentCamera(const std::string& cameraName);
	// カメラの転送
	static void TransferCamera(uint32_t rootParameterIndex);
#pragma endregion

#pragma region PunctualLightManager
	// ライトの追加
	static void AddPunctualLight(const std::string& lightName, const PunctualLightData& lightData = PunctualLightData{});
	// ライトの削除
	static void RemovePunctualLight(const std::string& lightName);
	// ライトの取得
	static PunctualLightData& GetLightData(const std::string& lightName);
	// ライトの転送
	static void TransferPunctualLight(uint32_t parmIndex);
#pragma endregion

#pragma region Renderer3DManager
	// 描画オブジェクトの追加
	// シンプル形状
	static void CreatePrimitiveRenderer3D(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName = "");
	// 静的モデル
	static void CreateStaticRenderer3D(const std::string& name, const std::string& modelName);
	// スキニングモデル
	static void CreateSkinningRenderer3D(const std::string& name, const std::string& modelName);
	// 描画オブジェクトの削除
	static void RemoveRenderer3D(const std::string& name);
	// 描画オブジェクトの取得
	static BaseRenderable3D* FindRenderer3D(const std::string& name);
#pragma endregion

#pragma region ColliderManager
	// コライダーの追加
	static void CreateCollider(const std::string& name, Collider3DType colliderType);
	// コライダーの削除
	static void RemoveCollider(const std::string& name);
	// コライダーの取得
	static BaseCollider3D* FindCollider(const std::string& name);
#pragma endregion

#pragma region Emitter3DManager
	// エミッターの追加
	static std::string CreateEmitter3D(const std::string& emitterName, const Vector3& position);
	// エミッターの削除
	static void RemoveEmitter3D(const std::string& emitterName);
	// エミッターの取得
	static Emitter3D* FindEmitter3D(const std::string& emitterName);
#pragma endregion

#pragma region ParticleGroup3DManager
	// シンプル形状パーティクルグループの追加
	static std::string CreatePrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName = "");
	// パーティクルグループの削除
	static void RemoveParticleGroup3D(const std::string& particleGraoupName);
	// パーティクルグループの取得
	static BaseParticleGroup3D* FindParticleGroup3D(const std::string& particleGraoupName);
#pragma endregion

#pragma region LineDrawer3D
	// ライン描画
	static void DrawLine3D(const Vector3& start, const Vector3& end, const RGBA& color);
#pragma endregion

#pragma region CollisionManager


#pragma endregion

#pragma region DataIO
	// コライダーファイルの読み込み
	static void LoadColliderDataFile(const std::string& fileName);

#pragma endregion 

#pragma region GUI


#pragma endregion

#pragma region PreRender
	// Object3Dの描画前処理
	static void PreDrawObject3D();

	// Object2Dの描画前処理
	static void PreDrawObject2D();

#pragma endregion

private: // メンバ変数
	// 終了リクエスト
	bool endRequest_ = false;
	// GUI描画フラグ
	bool isActivateGUI_ = false;

protected:
#ifdef _DEBUG
	static std::unique_ptr<D3DResourceLeakChecker> leakCheck_;
#endif // _DEBUG

	// 
	// BaseSystems
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
	static std::unique_ptr<ComputePipelineManager> computePipelineManager_;

	// 
	// AssetContainer
	// 
	static std::unique_ptr<TextureDataContainer> textureDataCantainer_;
	static std::unique_ptr<PrimitiveShapeDataContainer> primitiveDataContainer_;
	static std::unique_ptr<ModelDataContainer> modelDataContainer_;
	static std::unique_ptr<AnimationDataContainer> animationDataContainer_;
	static std::unique_ptr<SoundDataContainer> soundDataContainer_;

	//
	// ObjectManager
	//
	static std::unique_ptr<GameObject3DManager> gameObject3DManager_;
	static std::unique_ptr<Camera3DManager> camera3DManager_;
	static std::unique_ptr<Renderer3DManager> renderer3DManager_;
	static std::unique_ptr<PunctualLightManager> punctualLightManager_;
	static std::unique_ptr<ColliderManager> colliderManager_;
	static std::unique_ptr<Emitter3DManager> emitter3DManager_;
	static std::unique_ptr<ParticleGroup3DManager> particleGroup3DManager_;

	// 
	// Drawer
	// 
	static std::unique_ptr<LineDrawer3D> lineDrawer3D_;

	// 
	// GameManager
	// 
	static std::unique_ptr<CollisionManager> collisionManager_;
	static std::unique_ptr<SceneManager<GameData>> sceneManager_;

	//
	// Data入出力クラス
	//
	static std::unique_ptr<DataIO> dataIO_;

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