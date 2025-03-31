#include "GameObject3DGroup.h"

#include "Framework/MAGI.h"

using namespace MAGIMath;

GameObject3DGroup::GameObject3DGroup(GameObject3D* gameObject3D, const std::string& groupName) {
	assert(gameObject3D);
	thisGameObject_ = gameObject3D;
	name_ = groupName;
	CreateInstancingResource();
	MapInstancingData();
}

void GameObject3DGroup::Update() {
	// オブジェクトを更新
	for (auto& gameObject : gameObjects_) {
		gameObject.second->UpdateHierarchy();
		gameObject.second->UpdateWorldTransformHierarchy();
	}
	// Update
	UpdateData();
	// instance
	UpdateInstancingData();
}

void GameObject3DGroup::Draw() {
	// 描画前処理
	PrepareForRendering();
	// 描画
	thisGameObject_->DrawInstanced(instanceCount_);
}

void GameObject3DGroup::DeleteGarbage() {

}

std::string& GameObject3DGroup::GetName() {
	return name_;
}

void GameObject3DGroup::PrepareForRendering() {
	// コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = MAGISYSTEM::GetDirectXCommandList();
	// PSOを設定
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Object3D, blendMode_));
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	// StructuredBufferのSRVを設定する
	commandList->SetGraphicsRootDescriptorTable(1, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(srvIndex_));
	// カメラを転送
	MAGISYSTEM::TransferCamera(cameraRootParamaterIndex_);
	// ライトを転送
	MAGISYSTEM::TransferPunctualLight(lightRootParamaterIndex_);
}

void GameObject3DGroup::CreateInstancingResource() {
	// instancing用のリソースを作る
	instancingResource_ = MAGISYSTEM::CreateBufferResource(sizeof(GameObject3DDataForGPU) * kMaxInstance_);
	// srvのインデックスを割り当て
	srvIndex_ = MAGISYSTEM::SrvUavAllocate();
	// Srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(srvIndex_, instancingResource_.Get(), kMaxInstance_, sizeof(GameObject3DDataForGPU));
}

void GameObject3DGroup::MapInstancingData() {
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	for (uint32_t index = 0; index < kMaxInstance_; ++index) {
		instancingData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void GameObject3DGroup::UpdateInstancingData() {
	// 描画すべきインスタンス数
	instanceCount_ = 0;

	for (std::list<GameObject3DData>::iterator objectIterator = gameObjectsData_.begin();
		objectIterator != gameObjectsData_.end();) {

		if (instanceCount_ < kMaxInstance_) {
			// Wマトリックスを求める
			// Scale
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*objectIterator).transform.scale);
			// 
			Matrix4x4 rotateMatrix = MakeRotateXYZMatrix((*objectIterator).transform.rotate);

			// translate
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*objectIterator).transform.translate);

			// ワールド行列を作成
			Matrix4x4 worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;

			// ワールド行列
			instancingData_[instanceCount_].worldMatrix = worldMatrix;
			// 色を入力
			instancingData_[instanceCount_].color.x = (*objectIterator).color.x;
			instancingData_[instanceCount_].color.y = (*objectIterator).color.y;
			instancingData_[instanceCount_].color.z = (*objectIterator).color.z;
			instancingData_[instanceCount_].color.w = (*objectIterator).color.w;

			// 生きているParticleの数を1つカウントする
			instanceCount_++;
		}
		// 次のイテレーターに進める
		++objectIterator;
	}

}

void GameObject3DGroup::UpdateData() {

}
