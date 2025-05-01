// MeshDrawer.cpp ― Mesh Shader + Meshlet （DXMesh v168 10-bit 版）
#define NOMINMAX
#include "MeshDrawer.h"

#include <cassert>
#include <cstring>
#include <DirectXMath.h>

#include "Framework/MAGI.h"
#include "Logger/Logger.h"

using namespace DirectX;

constexpr uint32_t kMaxVertsPerMeshlet = 128;   // DXMesh デフォルト 128 でも可
constexpr uint32_t kMaxPrimsPerMeshlet = 128;  // DXMesh デフォルト 128 でも可

// ─────────────────────────────────────────────
MeshDrawer::MeshDrawer(const MeshData& meshData)
{
    /*=== 頂点 / インデックス ===================================================*/
    vertexCount_ = static_cast<uint32_t>(meshData.vertices.size());
    indexCount_  = static_cast<uint32_t>(meshData.indices.size());
    assert(vertexCount_ && indexCount_);

    vertexBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * vertexCount_);
    vertexSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIdx_, vertexBuffer_.Get(),
                                          vertexCount_, sizeof(VertexData3D));

    indexBuffer_  = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * indexCount_);
    indexSrvIdx_  = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(indexSrvIdx_, indexBuffer_.Get(),
                                          indexCount_, sizeof(uint32_t));

    // 転送
    vertexBuffer_->Map(0,nullptr,reinterpret_cast<void**>(&vertexData_));
    std::memcpy(vertexData_, meshData.vertices.data(),
                sizeof(VertexData3D)*vertexCount_);
    vertexBuffer_->Unmap(0,nullptr);

    indexBuffer_->Map(0,nullptr,reinterpret_cast<void**>(&indexData_));
    std::memcpy(indexData_, meshData.indices.data(),
                sizeof(uint32_t)*indexCount_);
    indexBuffer_->Unmap(0,nullptr);

    /*=== Meshlet 生成 =========================================================*/
    std::vector<Meshlet>         meshlets;
    std::vector<uint8_t>         uniqueVertBytes;    // 中身は uint32_t[]
    std::vector<MeshletTriangle> primIB;             // 10-bit ×3 構造体

    HRESULT hr = DirectX::ComputeMeshlets(
        meshData.indices.data(),                     // indices
        indexCount_ / 3u,                            // faceCount
        reinterpret_cast<const XMFLOAT3*>(meshData.vertices.data()),
        vertexCount_,
        nullptr,
        meshlets, uniqueVertBytes, primIB,
        kMaxVertsPerMeshlet, kMaxPrimsPerMeshlet);
    assert(SUCCEEDED(hr) && !meshlets.empty());

    meshletCount_ = static_cast<uint32_t>(meshlets.size());

    /*=== uniqueVert: byte → uint32 配列へ ======================================*/
    uniqueVertCount_ = static_cast<uint32_t>(uniqueVertBytes.size() / sizeof(uint32_t));
    std::vector<uint32_t> uniqueVert32(uniqueVertCount_);
    std::memcpy(uniqueVert32.data(), uniqueVertBytes.data(), uniqueVertBytes.size());

    const uint32_t primCount = static_cast<uint32_t>(primIB.size());

    /*=== GPU バッファ & SRV ===================================================*/
    auto NonZero = [](size_t bytes){ return bytes ? bytes : size_t(4); };

    // Meshlet ヘッダ
    meshletBuffer_ = MAGISYSTEM::CreateBufferResource(
        NonZero(sizeof(Meshlet)*meshletCount_));
    meshletSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(
        meshletSrvIdx_, meshletBuffer_.Get(),
        std::max(meshletCount_,1u), sizeof(Meshlet));

    // Unique 頂点 (uint32)
    meshletUniqueVertIB_ = MAGISYSTEM::CreateBufferResource(
        NonZero(sizeof(uint32_t)*uniqueVertCount_));
    uniqueVertSrvIdx_    = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(
        uniqueVertSrvIdx_, meshletUniqueVertIB_.Get(),
        std::max(uniqueVertCount_,1u), sizeof(uint32_t));

    // MeshletTriangle (10-bit packed 構造体 ＝ 4 Byte)
    meshletPrimIB_ = MAGISYSTEM::CreateBufferResource(
        NonZero(sizeof(MeshletTriangle)*primCount));
    primSrvIdx_    = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(
        primSrvIdx_, meshletPrimIB_.Get(),
        std::max(primCount,1u), sizeof(MeshletTriangle));

    // 転送
    void* p=nullptr;
    meshletBuffer_->Map(0,nullptr,&p);
    std::memcpy(p, meshlets.data(), sizeof(Meshlet)*meshletCount_);
    meshletBuffer_->Unmap(0,nullptr);

    meshletUniqueVertIB_->Map(0,nullptr,&p);
    std::memcpy(p, uniqueVert32.data(), sizeof(uint32_t)*uniqueVertCount_);
    meshletUniqueVertIB_->Unmap(0,nullptr);

    meshletPrimIB_->Map(0,nullptr,&p);
    std::memcpy(p, primIB.data(), sizeof(MeshletTriangle)*primCount);
    meshletPrimIB_->Unmap(0,nullptr);

    /*=== マテリアル ===========================================================*/
    materialBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(ModelMaterialDataForGPU));
    materialBuffer_->Map(0,nullptr,reinterpret_cast<void**>(&material_));
    *material_ = {
        .textureIndex = MAGISYSTEM::GetTextureIndex(meshData.material.textureFilePath),
        .baseColor    = meshData.material.color,
        .uvMatrix     = MAGIMath::MakeIdentityMatrix4x4(),
    };
    materialBuffer_->Unmap(0,nullptr);
}

MeshDrawer::~MeshDrawer() = default;
void MeshDrawer::Update() {}

// -----------------------------------------------------------------------------
void MeshDrawer::Draw(uint32_t instanceCount)
{
    if (!instanceCount || !meshletCount_) return;

    auto* cmd = MAGISYSTEM::GetDirectXCommandList6();

    cmd->SetGraphicsRootConstantBufferView(2, materialBuffer_->GetGPUVirtualAddress());
    cmd->SetGraphicsRootDescriptorTable(4, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(5, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(meshletSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(6, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(uniqueVertSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(7, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(primSrvIdx_));

    cmd->DispatchMesh(meshletCount_, instanceCount, 1);
}
