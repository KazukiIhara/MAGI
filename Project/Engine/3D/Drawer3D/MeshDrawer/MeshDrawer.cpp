#include "MeshDrawer.h"

// C++
#include <cassert>
#include <DirectXMath.h>

// MAGI
#include "Enums/BlendModeEnum.h"
#include "Framework/MAGI.h"
#include "Logger/Logger.h"

using namespace DirectX;

// ───────────── 定数 ─────────────
constexpr uint32_t kMaxVertsPerMeshlet = 64;
constexpr uint32_t kMaxPrimsPerMeshlet = 126;

// ───────────── ctor ─────────────
MeshDrawer::MeshDrawer(const MeshData& meshData)
{
    /*=========== 頂点／インデックス ===========*/
    vertexCount_ = static_cast<uint32_t>(meshData.vertices.size());
    assert(vertexCount_ > 0);

    indexCount_  = static_cast<uint32_t>(meshData.indices.size());
    assert(indexCount_ > 0);

    vertexBuffer_  = MAGISYSTEM::CreateBufferResource(sizeof(VertexData3D) * vertexCount_);
    vertexSrvIdx_  = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(vertexSrvIdx_, vertexBuffer_.Get(),
                                          vertexCount_, sizeof(VertexData3D));

    indexBuffer_   = MAGISYSTEM::CreateBufferResource(sizeof(uint32_t) * indexCount_);
    indexSrvIdx_   = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(indexSrvIdx_, indexBuffer_.Get(),
                                          indexCount_, sizeof(uint32_t));

    /* upload data */
    {
        vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
        std::memcpy(vertexData_, meshData.vertices.data(),
                    sizeof(VertexData3D) * vertexCount_);
        vertexBuffer_->Unmap(0, nullptr);

        indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
        std::memcpy(indexData_, meshData.indices.data(),
                    sizeof(uint32_t) * indexCount_);
        indexBuffer_->Unmap(0, nullptr);
    }

    /*=========== Meshlet 生成 (DirectXMesh) ===========*/
    std::vector<Meshlet>            meshlets;
    std::vector<uint8_t>            uniqueVertIB;
    std::vector<MeshletTriangle>    primIB;

    HRESULT hr = ComputeMeshlets(
        meshData.indices.data(),          // indices
        indexCount_ / 3,                  // faceCount
        reinterpret_cast<const XMFLOAT3*>(
             meshData.vertices.data()),   // positions
        vertexCount_,
        nullptr,                          // adjacency (省略可)
        meshlets, uniqueVertIB, primIB,
        kMaxVertsPerMeshlet, kMaxPrimsPerMeshlet);

    if (FAILED(hr) || meshlets.empty()) {
        Logger::Log("ComputeMeshlets failed — check mesh topology.\n");
        assert(false && "Meshlet generation failed");
    }

    meshletCount_ = static_cast<uint32_t>(meshlets.size());

    /*=========== GPU バッファ & SRV ===========*/
    // Meshlet
    meshletBuffer_ = MAGISYSTEM::CreateBufferResource(
        sizeof(Meshlet) * meshletCount_);
    meshletSrvIdx_ = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(
        meshletSrvIdx_, meshletBuffer_.Get(),
        meshletCount_, sizeof(Meshlet));

    // Unique Vertex Indices (ByteAddressBuffer)
    meshletUniqueVertIB_ = MAGISYSTEM::CreateBufferResource(uniqueVertIB.size());
    uniqueVertSrvIdx_    = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvByteAddressBuffer(
        uniqueVertSrvIdx_, meshletUniqueVertIB_.Get(),
        static_cast<uint32_t>(uniqueVertIB.size()));

    // Primitive Indices
    meshletPrimIB_ = MAGISYSTEM::CreateBufferResource(
        sizeof(MeshletTriangle) * primIB.size());
    primSrvIdx_    = MAGISYSTEM::SrvUavAllocate();
    MAGISYSTEM::CreateSrvStructuredBuffer(
        primSrvIdx_, meshletPrimIB_.Get(),
        static_cast<uint32_t>(primIB.size()), sizeof(MeshletTriangle));

    /* upload */
    {
        void* p = nullptr;
        meshletBuffer_->Map(0, nullptr, &p);
        std::memcpy(p, meshlets.data(), sizeof(Meshlet) * meshletCount_);
        meshletBuffer_->Unmap(0, nullptr);

        meshletUniqueVertIB_->Map(0, nullptr, &p);
        std::memcpy(p, uniqueVertIB.data(), uniqueVertIB.size());
        meshletUniqueVertIB_->Unmap(0, nullptr);

        meshletPrimIB_->Map(0, nullptr, &p);
        std::memcpy(p, primIB.data(), sizeof(MeshletTriangle) * primIB.size());
        meshletPrimIB_->Unmap(0, nullptr);
    }

    /*=========== マテリアル ===========*/
    materialBuffer_ = MAGISYSTEM::CreateBufferResource(sizeof(ModelMaterialDataForGPU));
    materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
    *material_ = {
        .textureIndex = MAGISYSTEM::GetTextureIndex(meshData.material.textureFilePath),
        .baseColor    = meshData.material.color,
        .uvMatrix     = MAGIMath::MakeIdentityMatrix4x4(),
    };
    materialBuffer_->Unmap(0, nullptr);
}

MeshDrawer::~MeshDrawer() = default;
void MeshDrawer::Update() {}

void MeshDrawer::Draw(uint32_t instanceCount)
{
    if (instanceCount == 0) return;

    ID3D12GraphicsCommandList6* cmd = MAGISYSTEM::GetDirectXCommandList6();

    /* b1: マテリアル CBV */
    cmd->SetGraphicsRootConstantBufferView(1, materialBuffer_->GetGPUVirtualAddress());

    /* t1–t5: SRV バインド */
    cmd->SetGraphicsRootDescriptorTable(2, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(vertexSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(indexSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(4, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(meshletSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(5, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(uniqueVertSrvIdx_));
    cmd->SetGraphicsRootDescriptorTable(6, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(primSrvIdx_));

    /* DispatchMesh */
    cmd->DispatchMesh(meshletCount_, instanceCount, 1);
}
