#include "MAGI.h"

#include "Logger/Logger.h"

// Staticメンバ変数の初期化
#ifdef _DEBUG
std::unique_ptr<D3DResourceLeakChecker> MAGISYSTEM::leakCheck_ = nullptr;
#endif // _DEBUG

// 
// BaseSystems
// 
std::unique_ptr<WindowApp> MAGISYSTEM::windowApp_ = nullptr;
std::unique_ptr<DeltaTimer> MAGISYSTEM::deltaTimer_ = nullptr;
std::unique_ptr<DirectInput> MAGISYSTEM::directInput_ = nullptr;

// 
// DirectXBaseSystems
// 
std::unique_ptr<DXGI> MAGISYSTEM::dxgi_ = nullptr;
std::unique_ptr<DirectXCommand> MAGISYSTEM::directXCommand_ = nullptr;
std::unique_ptr<Fence> MAGISYSTEM::fence_ = nullptr;
std::unique_ptr<ShaderCompiler> MAGISYSTEM::shaderCompiler_ = nullptr;

// 
// ViewManagers
// 
std::unique_ptr<RTVManager> MAGISYSTEM::rtvManager_ = nullptr;
std::unique_ptr<DSVManager> MAGISYSTEM::dsvManager_ = nullptr;
std::unique_ptr<SRVUAVManager> MAGISYSTEM::srvuavManager_ = nullptr;

// 
// DirectXRenderSystems
// 
std::unique_ptr<SwapChain> MAGISYSTEM::swapChain_ = nullptr;
std::unique_ptr<DepthStencil> MAGISYSTEM::depthStencil_ = nullptr;
std::unique_ptr<ResourceBarrier> MAGISYSTEM::resourceBarrier_ = nullptr;
std::unique_ptr<RenderTarget> MAGISYSTEM::renderTarget_ = nullptr;
std::unique_ptr<Viewport> MAGISYSTEM::viewport_ = nullptr;
std::unique_ptr<ScissorRect> MAGISYSTEM::scissorRect_ = nullptr;

//
// PipelineManager
//
std::unique_ptr<GraphicsPipelineManager> MAGISYSTEM::graphicsPipelineManager_ = nullptr;
std::unique_ptr<ComputePipelineManager> MAGISYSTEM::computePipelineManager_ = nullptr;

// 
// AssetContainer
// 
std::unique_ptr<TextureDataContainer> MAGISYSTEM::textureDataCantainer_ = nullptr;
std::unique_ptr<PrimitiveShapeDataContainer> MAGISYSTEM::primitiveDataContainer_ = nullptr;
std::unique_ptr<ModelDataContainer> MAGISYSTEM::modelDataContainer_ = nullptr;
std::unique_ptr<AnimationDataContainer> MAGISYSTEM::animationDataContainer_ = nullptr;
std::unique_ptr<SoundDataContainer> MAGISYSTEM::soundDataContainer_ = nullptr;

//
// ObjectManager
//
std::unique_ptr<GameObject3DManager> MAGISYSTEM::gameObject3DManager_ = nullptr;
std::unique_ptr<Camera3DManager> MAGISYSTEM::camera3DManager_ = nullptr;
std::unique_ptr<PunctualLightManager> MAGISYSTEM::punctualLightManager_ = nullptr;
std::unique_ptr<Renderer3DManager> MAGISYSTEM::renderer3DManager_ = nullptr;
std::unique_ptr<ColliderManager> MAGISYSTEM::colliderManager_ = nullptr;
std::unique_ptr<Emitter3DManager> MAGISYSTEM::emitter3DManager_ = nullptr;
std::unique_ptr<ParticleGroup3DManager> MAGISYSTEM::particleGroup3DManager_ = nullptr;

// 
// Drawer
// 
std::unique_ptr<LineDrawer3D> MAGISYSTEM::lineDrawer3D_ = nullptr;

// 
// GameManager
// 
std::unique_ptr<CollisionManager> MAGISYSTEM::collisionManager_ = nullptr;
std::unique_ptr<SceneManager<GameData>> MAGISYSTEM::sceneManager_ = nullptr;

//
// Data入出力クラス
//
std::unique_ptr<DataIO> MAGISYSTEM::dataIO_ = nullptr;

//
// UIクラス
//
std::unique_ptr<ImGuiController> MAGISYSTEM::imguiController_ = nullptr;
std::unique_ptr<GUI> MAGISYSTEM::gui_ = nullptr;

void MAGISYSTEM::Initialize() {
	// 開始ログ
	Logger::Log("MAGISYSTEM Start\n");

#ifdef _DEBUG
	// リークチェッカ
	leakCheck_ = std::make_unique<D3DResourceLeakChecker>();
#endif // _DEBUG

	// WindowApp
	windowApp_ = std::make_unique<WindowApp>();
	// DeltaTimer
	deltaTimer_ = std::make_unique<DeltaTimer>();
	// DirectInput
	directInput_ = std::make_unique<DirectInput>(windowApp_.get());


	// DXGI
	dxgi_ = std::make_unique<DXGI>();
	// DirectXCommand
	directXCommand_ = std::make_unique<DirectXCommand>(dxgi_.get());
	// Fence
	fence_ = std::make_unique<Fence>(dxgi_.get(), directXCommand_.get());
	// ShaderCompiler
	shaderCompiler_ = std::make_unique<ShaderCompiler>();


	// RTVManager
	rtvManager_ = std::make_unique<RTVManager>(dxgi_.get());
	// DSVManager
	dsvManager_ = std::make_unique<DSVManager>(dxgi_.get());
	// SRVUAVmanager
	srvuavManager_ = std::make_unique<SRVUAVManager>(dxgi_.get());


	// SwapChain
	swapChain_ = std::make_unique<SwapChain>(windowApp_.get(), dxgi_.get(), directXCommand_.get(), rtvManager_.get());
	// DepthStencil
	depthStencil_ = std::make_unique<DepthStencil>(dxgi_.get(), directXCommand_.get(), dsvManager_.get());
	// ResouceBarrier
	resourceBarrier_ = std::make_unique<ResourceBarrier>(directXCommand_.get(), swapChain_.get());
	// RenderTarget
	renderTarget_ = std::make_unique<RenderTarget>(directXCommand_.get(), swapChain_.get(), depthStencil_.get());
	// Viewport
	viewport_ = std::make_unique<Viewport>(directXCommand_.get());
	// Scissor
	scissorRect_ = std::make_unique<ScissorRect>(directXCommand_.get());


	// TextureDataContainer
	textureDataCantainer_ = std::make_unique<TextureDataContainer>(dxgi_.get(), directXCommand_.get(), fence_.get(), srvuavManager_.get());
	// PrimitiveDataContainer
	primitiveDataContainer_ = std::make_unique<PrimitiveShapeDataContainer>();
	// ModelDataContainer
	modelDataContainer_ = std::make_unique<ModelDataContainer>(textureDataCantainer_.get());
	// AnimationDataContainer
	animationDataContainer_ = std::make_unique<AnimationDataContainer>();
	// SoundDataCOntainer
	soundDataContainer_ = std::make_unique<SoundDataContainer>();

	// GraphicsPipelineManager
	graphicsPipelineManager_ = std::make_unique<GraphicsPipelineManager>(dxgi_.get(), shaderCompiler_.get());
	// ComputePipelineManager
	computePipelineManager_ = std::make_unique<ComputePipelineManager>(dxgi_.get(), shaderCompiler_.get());


	// GameObject3DManager
	gameObject3DManager_ = std::make_unique<GameObject3DManager>();
	// Renderer3DManager
	renderer3DManager_ = std::make_unique<Renderer3DManager>();
	// Camera3DManager
	camera3DManager_ = std::make_unique<Camera3DManager>();
	// PunctualLightManager
	punctualLightManager_ = std::make_unique<PunctualLightManager>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get());
	// ColliderManager
	colliderManager_ = std::make_unique<ColliderManager>();
	// Emitter3DManager
	emitter3DManager_ = std::make_unique<Emitter3DManager>();
	// ParticleGroup3DManager
	particleGroup3DManager_ = std::make_unique<ParticleGroup3DManager>();


	// LineDrawer3D
	lineDrawer3D_ = std::make_unique<LineDrawer3D>(dxgi_.get(), directXCommand_.get(), srvuavManager_.get(), graphicsPipelineManager_.get(), camera3DManager_.get());


	// CollisionManager
	collisionManager_ = std::make_unique<CollisionManager>(colliderManager_.get());
	// SceneManager
	sceneManager_ = std::make_unique<SceneManager<GameData>>();


	// DataIO
	dataIO_ = std::make_unique<DataIO>(renderer3DManager_.get(), colliderManager_.get(), gameObject3DManager_.get());


	// ImGuiController
	imguiController_ = std::make_unique<ImGuiController>(windowApp_.get(), dxgi_.get(), directXCommand_.get(), srvuavManager_.get());

	// GUI
	gui_ = std::make_unique<GUI>(deltaTimer_.get(), srvuavManager_.get(), dataIO_.get(), textureDataCantainer_.get());

	// 初期化完了ログ
	Logger::Log("MAGISYSTEM Initialize\n");
}

void MAGISYSTEM::Finalize() {

	// GUI
	if (gui_) {
		gui_.reset();
	}

	// ImGuiController
	if (imguiController_) {
		imguiController_.reset();
	}

	// DataIO
	if (dataIO_) {
		dataIO_.reset();
	}

	// SceneManager
	if (sceneManager_) {
		sceneManager_.reset();
	}

	// CollisionManager
	if (collisionManager_) {
		collisionManager_.reset();
	}

	// LineDrawer3D
	if (lineDrawer3D_) {
		lineDrawer3D_.reset();
	}

	// ParticleGroup3DManager
	if (particleGroup3DManager_) {
		particleGroup3DManager_.reset();
	}

	// Emitter3DManager
	if (emitter3DManager_) {
		emitter3DManager_.reset();
	}

	// ColliderManager
	if (colliderManager_) {
		colliderManager_.reset();
	}

	// Renderer3DManager
	if (renderer3DManager_) {
		renderer3DManager_.reset();
	}

	// PunctualLightManager
	if (punctualLightManager_) {
		punctualLightManager_.reset();
	}

	// Camera3DManager
	if (camera3DManager_) {
		camera3DManager_.reset();
	}

	// GameObject3DManager
	if (gameObject3DManager_) {
		gameObject3DManager_.reset();
	}

	// CompuetPipelineManager
	if (computePipelineManager_) {
		computePipelineManager_.reset();
	}

	// GraphicsPipelineManager
	if (graphicsPipelineManager_) {
		graphicsPipelineManager_.reset();
	}

	// SoundDataContainer
	if (soundDataContainer_) {
		soundDataContainer_.reset();
	}

	// AnimationDataContainer
	if (animationDataContainer_) {
		animationDataContainer_.reset();
	}

	// ModelDataContainer
	if (modelDataContainer_) {
		modelDataContainer_.reset();
	}

	// PrimitiveDataContainer
	if (primitiveDataContainer_) {
		primitiveDataContainer_.reset();
	}

	// TextureDataContainer
	if (textureDataCantainer_) {
		textureDataCantainer_.reset();
	}

	// Scissor
	if (scissorRect_) {
		scissorRect_.reset();
	}

	// Viewport
	if (viewport_) {
		viewport_.reset();
	}

	// RenderTarget
	if (renderTarget_) {
		renderTarget_.reset();
	}

	// ResourceBarrier
	if (resourceBarrier_) {
		resourceBarrier_.reset();
	}

	// DepthStencil
	if (depthStencil_) {
		depthStencil_.reset();
	}

	// SwapChain
	if (swapChain_) {
		swapChain_.reset();
	}

	// SRVUAVManager
	if (srvuavManager_) {
		srvuavManager_.reset();
	}

	// DSVManager
	if (dsvManager_) {
		dsvManager_.reset();
	}

	// RTVManager
	if (rtvManager_) {
		rtvManager_.reset();
	}

	// ShaderCompiler
	if (shaderCompiler_) {
		shaderCompiler_.reset();
	}

	// Fence
	if (fence_) {
		fence_.reset();
	}

	// DirectXCommand
	if (directXCommand_) {
		directXCommand_.reset();
	}

	// DXGI
	if (dxgi_) {
		dxgi_.reset();
	}

	// DirectInput
	if (directInput_) {
		directInput_.reset();
	}

	// DeltaTimer
	if (deltaTimer_) {
		deltaTimer_.reset();
	}

	// WindowApp
	if (windowApp_) {
		windowApp_.reset();
	}

	// 終了ログ
	Logger::Log("MAGISYSTEM Finalize\n");
}

void MAGISYSTEM::Update() {
	// デルタタイムクラスを更新
	deltaTimer_->Update();

	// ウィンドウにメッセージが来ていたら最優先で処理
	if (windowApp_->Update()) {
		endRequest_ = true;
	}

	// 入力の更新
	directInput_->Update();

	// F11キーでフルスクリーンの切り替え処理
	if (directInput_->TriggerKey(DIK_F11)) {
		windowApp_->ToggleFullScreen();
	}

	// escキーが入力されたらアプリケーションを終了
	if (directInput_->PushKey(DIK_ESCAPE)) {
		endRequest_ = true;
	}

	// デバッグモード切替
	if (directInput_->TriggerKey(DIK_P)) {
		camera3DManager_->GetIsDebugCamera() = !camera3DManager_->GetIsDebugCamera();
	}

	// ImGui開始処理
	imguiController_->BeginFrame();

	// Dataクラスフレーム開始処理
	dataIO_->BeginFrame();

	// シーンの更新処理
	sceneManager_->Update();

	// ゲームオブジェクトマネージャの更新
	gameObject3DManager_->Update();

	// カメラマネージャの更新処理
	camera3DManager_->Update();

	// ライトマネージャの更新
	punctualLightManager_->Update();

	// 描画オブジェクトクラスの更新処理
	renderer3DManager_->Update();

	// コライダーマネージャの更新
	colliderManager_->Update();

	// 3Dエミッターマネージャの更新処理
	emitter3DManager_->Update();

	// 3Dパーティクルグループマネージャの更新処理
	particleGroup3DManager_->Update();

	// コリジョンマネージャの更新処理
	collisionManager_->Update();


	// 3Dライン描画クラスの更新
	lineDrawer3D_->Update();

	// Dataクラスフレーム終了処理
	dataIO_->EndFrame();

	// GUI更新処理
	gui_->Update();

	// ImGui内部コマンド生成
	imguiController_->EndFrame();
}

void MAGISYSTEM::Draw() {

	// 
	// DirectX描画前処理
	// 

	// リソースバリアを設定
	resourceBarrier_->PreDrawSwapChainResourceBarrierTransition();
	// レンダーターゲットをスワップチェーンに設定
	renderTarget_->SetRenderTarget(RenderTargetType::SwapChain);
	// 深度をクリア
	depthStencil_->ClearDepthView();
	// レンダーターゲットをクリア
	renderTarget_->ClearRenderTarget(RenderTargetType::SwapChain);
	// ビューポートの設定
	viewport_->SettingViewport();
	// シザー矩形の設定
	scissorRect_->SettingScissorRect();

	// コマンドリスト取得
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();

	// SRVUAVのディスクリプタヒープを設定
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvuavManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// 
	// 描画処理
	// 


	//
	// シーンの描画処理
	//
	sceneManager_->Draw();


	// 
	// ゲームオブジェクトマネージャの描画処理(まだない)
	// 

	gameObject3DManager_->Draw();

	// 
	// Object3Dの描画前処理
	// 
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Object3D));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 3Dオブジェクト描画処理
	renderer3DManager_->Draw();

	// 
	// LineDrawer3Dの描画前処理
	// 
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Line3D));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// 
	// LineDrawer3Dの描画処理
	// 
	lineDrawer3D_->Draw();

	//
	// ParticleGroup3Dの描画前処理
	//
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Particle3D));
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	// ParticleGroup3Dの描画処理
	//
	particleGroup3DManager_->Draw();

	//
	// ImGui描画処理
	//

	imguiController_->Draw();


	// 
	// DirectX描画後処理
	// 

	// リソースバリアを描画後の状態にする
	resourceBarrier_->PostDrawSwapChainResourceBarrierTransition();
	// コマンドを閉じて実行
	directXCommand_->KickCommand();
	// GPUとOSに画面の交換を行うように通知
	swapChain_->Present();
	// Fenceによる待機
	fence_->WaitGPU();
	// 次のフレーム用にコマンドをリセット
	directXCommand_->ResetCommand();
}

void MAGISYSTEM::Run() {
	// 初期化
	Initialize();
	// メインループ
	while (true) {
		// 更新
		Update();

		// 終了リクエストがあったらループを抜ける;
		if (IsEndRequest()) {
			break;
		}

		// 描画
		Draw();
	}

	// 終了
	Finalize();
}

bool MAGISYSTEM::IsEndRequest() const {
	return endRequest_;
}

HWND MAGISYSTEM::GetWindowHandle() {
	return windowApp_->GetHwnd();
}

float MAGISYSTEM::GetDeltaTime() {
	return deltaTimer_->GetDeltaTime();
}

bool MAGISYSTEM::PushKey(BYTE keyNumber) {
	return directInput_->PushKey(keyNumber);
}

bool MAGISYSTEM::TriggerKey(BYTE keyNumber) {
	return directInput_->TriggerKey(keyNumber);
}

bool MAGISYSTEM::HoldKey(BYTE keyNumber) {
	return directInput_->HoldKey(keyNumber);
}

bool MAGISYSTEM::ReleaseKey(BYTE keyNumber) {
	return directInput_->ReleaseKey(keyNumber);
}

bool MAGISYSTEM::PushMouseButton(MouseButton mouseButton) {
	return directInput_->PushMouseButton(mouseButton);
}

bool MAGISYSTEM::TriggerMouseButton(MouseButton mouseButton) {
	return directInput_->TriggerMouseButton(mouseButton);
}

bool MAGISYSTEM::HoldMouseButton(MouseButton mouseButton) {
	return directInput_->HoldMouseButton(mouseButton);
}

bool MAGISYSTEM::ReleaseMouseButton(MouseButton mouseButton) {
	return directInput_->ReleaseMouseButton(mouseButton);
}

int64_t MAGISYSTEM::GetMouseWheelDelta() {
	return directInput_->GetMouseWheelDelta();
}

ID3D12Device* MAGISYSTEM::GetDirectXDevice() {
	return dxgi_->GetDevice();
}

ComPtr<ID3D12Resource> MAGISYSTEM::CreateBufferResource(size_t sizeInBytes, bool isUav) {
	return dxgi_->CreateBufferResource(sizeInBytes, isUav);
}

ID3D12GraphicsCommandList* MAGISYSTEM::GetDirectXCommandList() {
	return directXCommand_->GetList();
}

void MAGISYSTEM::KickCommand() {
	directXCommand_->KickCommand();
}

void MAGISYSTEM::ResetCommand() {
	directXCommand_->ResetCommand();
}

void MAGISYSTEM::WaitGPU() {
	fence_->WaitGPU();
}

ID3D12DescriptorHeap* MAGISYSTEM::GetSrvUavDescriptorHeap() {
	return srvuavManager_->GetDescriptorHeap();
}

D3D12_CPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetSrvUavDescriptorHandleCPU(uint32_t index) {
	return srvuavManager_->GetDescriptorHandleCPU(index);
}

D3D12_GPU_DESCRIPTOR_HANDLE MAGISYSTEM::GetSrvUavDescriptorHandleGPU(uint32_t index) {
	return srvuavManager_->GetDescriptorHandleGPU(index);
}

uint32_t MAGISYSTEM::ViewAllocate() {
	return srvuavManager_->Allocate();
}

void MAGISYSTEM::CreateSrvStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvuavManager_->CreateSrvStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

void MAGISYSTEM::CreateUavStructuredBuffer(uint32_t viewIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {
	srvuavManager_->CreateUavStructuredBuffer(viewIndex, pResource, numElements, structureByteStride);
}

ID3D12PipelineState* MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType pipelineState, BlendMode blendMode) {
	return graphicsPipelineManager_->GetPipelineState(pipelineState, blendMode);
}

ID3D12RootSignature* MAGISYSTEM::GetComputeRootSignature(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetRootSignature(pipelineState);
}

ID3D12PipelineState* MAGISYSTEM::GetComputePipelineState(ComputePipelineStateType pipelineState) {
	return computePipelineManager_->GetPipelineState(pipelineState);
}

void MAGISYSTEM::LoadTexture(const std::string& filePath, bool isFullPath) {
	textureDataCantainer_->Load(filePath, isFullPath);
}

void MAGISYSTEM::LoadNormalMapTexture(const std::string& filePath) {
	textureDataCantainer_->LoadNormalMap(filePath);
}

std::map<std::string, Texture>& MAGISYSTEM::GetTexture() {
	return textureDataCantainer_->GetTexture();
}

const DirectX::TexMetadata& MAGISYSTEM::GetTextureMetaData(const std::string& filePath) {
	return textureDataCantainer_->GetMetaData(filePath);
}

PrimitiveData MAGISYSTEM::GetPrimitiveShape(const Primitive3DType& primitive3dType) {
	return primitiveDataContainer_->GetPrimitiveShapeData(primitive3dType);
}

void MAGISYSTEM::LoadModel(const std::string& modelName) {
	modelDataContainer_->Load(modelName);
}

ModelData MAGISYSTEM::FindModel(const std::string& modelName) {
	return modelDataContainer_->FindModelData(modelName);
}

void MAGISYSTEM::LoadAnimation(const std::string& animationFileName, bool isInSameDirectoryAsModel) {
	animationDataContainer_->Load(animationFileName, isInSameDirectoryAsModel);
}

AnimationData MAGISYSTEM::FindAnimation(const std::string& animationName) {
	return animationDataContainer_->FindAnimationData(animationName);
}

void MAGISYSTEM::LoadWaveSound(const std::string& fileName) {
	soundDataContainer_->LoadWave(fileName);
}

void MAGISYSTEM::PlayWaveSound(const std::string& fileName) {
	soundDataContainer_->PlayWave(fileName);
}


void MAGISYSTEM::PlayLoopWaveSound(const std::string& fileName) {
	soundDataContainer_->PlayWaveLoop(fileName);
}

void MAGISYSTEM::StopWaveSound(const std::string& fileName) {
	soundDataContainer_->StopWave(fileName);
}

void MAGISYSTEM::StopLoopWaveSound(const std::string& fileName) {
	soundDataContainer_->StopWaveLoop(fileName);
}

void MAGISYSTEM::TransferCamera(uint32_t rootParameterIndex) {
	camera3DManager_->TransferCurrentCamera(rootParameterIndex);
}

void MAGISYSTEM::AddPunctualLight(const std::string& lightName, const PunctualLightData& lightData) {
	punctualLightManager_->AddNewLight(lightName, lightData);
}

void MAGISYSTEM::RemovePunctualLight(const std::string& lightName) {
	punctualLightManager_->RemoveLight(lightName);
}

PunctualLightData& MAGISYSTEM::GetLightData(const std::string& lightName) {
	return punctualLightManager_->GetPunctualLight(lightName);
}


void MAGISYSTEM::TransferPunctualLight(uint32_t parmIndex) {
	punctualLightManager_->TransferLightsData(parmIndex);
}

void MAGISYSTEM::CreatePrimitiveRenderer3D(const std::string& name, Primitive3DType primitiveRenderer, const std::string& textureName) {
	renderer3DManager_->CreatePrimitiveRenderer(name, primitiveRenderer, textureName);
}

void MAGISYSTEM::CreateStaticRenderer3D(const std::string& name, const std::string& modelName) {
	renderer3DManager_->CreateStaticRenderer(name, modelName);
}

void MAGISYSTEM::CreateSkinningRenderer3D(const std::string& name, const std::string& modelName) {
	renderer3DManager_->CreateSkinningRenderer(name, modelName);
}

void MAGISYSTEM::RemoveRenderer3D(const std::string& name) {
	renderer3DManager_->Remove(name);
}

BaseRenderable3D* MAGISYSTEM::FindRenderer3D(const std::string& name) {
	return renderer3DManager_->Find(name);
}

void MAGISYSTEM::CreateCollider(const std::string& name, Collider3DType colliderType) {
	colliderManager_->Create(name, colliderType);
}

void MAGISYSTEM::RemoveCollider(const std::string& name) {
	colliderManager_->Remove(name);
}

BaseCollider3D* MAGISYSTEM::FindCollider(const std::string& name) {
	return colliderManager_->Find(name);
}

std::string MAGISYSTEM::CreateEmitter3D(const std::string& emitterName, const Vector3& position) {
	return emitter3DManager_->CreateEmitter(emitterName, position);
}

void MAGISYSTEM::RemoveEmitter3D(const std::string& emitterName) {
	emitter3DManager_->Remove(emitterName);
}

Emitter3D* MAGISYSTEM::FindEmitter3D(const std::string& emitterName) {
	return emitter3DManager_->Find(emitterName);
}

std::string MAGISYSTEM::CreatePrimitiveParticleGroup3D(const std::string& particleGroupName, const Primitive3DType& primitiveType, const std::string& textureName) {
	return particleGroup3DManager_->CreatePrimitiveParticleGroup(particleGroupName, primitiveType, textureName);
}

void MAGISYSTEM::RemoveParticleGroup3D(const std::string& particleGraoupName) {
	return particleGroup3DManager_->Remove(particleGraoupName);
}

BaseParticleGroup3D* MAGISYSTEM::FindParticleGroup3D(const std::string& particleGraoupName) {
	return particleGroup3DManager_->Find(particleGraoupName);
}

void MAGISYSTEM::AddGameObject3D(std::unique_ptr<GameObject3D> newGameObject3D) {
	gameObject3DManager_->Add(std::move(newGameObject3D));
}

void MAGISYSTEM::RemoveGameObject3D(const std::string& objectName) {
	gameObject3DManager_->Remove(objectName);
}

GameObject3D* MAGISYSTEM::FindGameObject3D(const std::string& objectName) {
	return gameObject3DManager_->Find(objectName);
}

void MAGISYSTEM::AddCamera3D(std::unique_ptr<Camera3D> newCamera3D) {
	camera3DManager_->Add(std::move(newCamera3D));
}

void MAGISYSTEM::RemoveCamera3D(const std::string& cameraName) {
	camera3DManager_->Remove(cameraName);
}

Camera3D* MAGISYSTEM::FindCamera3D(const std::string& cameraName) {
	return camera3DManager_->Find(cameraName);
}

void MAGISYSTEM::SetCurrentCamera(const std::string& cameraName) {
	camera3DManager_->SetCurrentCameraName(cameraName);
}

void MAGISYSTEM::DrawLine3D(const Vector3& start, const Vector3& end, const RGBA& color) {
	lineDrawer3D_->AddLine(start, end, color);
}

void MAGISYSTEM::LoadColliderDataFile(const std::string& fileName) {
	dataIO_->LoadColliderDataFile(fileName);
}

void MAGISYSTEM::PreDrawObject3D() {
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// RootSignatureの設定
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Object3D));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MAGISYSTEM::PreDrawObject2D() {
	ID3D12GraphicsCommandList* commandList = directXCommand_->GetList();
	// RootSignatureの設定
	commandList->SetGraphicsRootSignature(graphicsPipelineManager_->GetRootSignature(GraphicsPipelineStateType::Object2D));
	// 形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}