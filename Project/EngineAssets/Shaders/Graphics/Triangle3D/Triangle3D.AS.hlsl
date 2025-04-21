#include "Triangle3D.hlsli"

// Mesh へ渡す情報
struct ASPayload
{
    uint instanceID;
};

ConstantBuffer<RootConstants> gRootConstant : register(b1);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID,
          out ASPayload payload)       // ← out で宣言
{
    // ベース + ローカルスレッド ID でインスタンスを一意に
    payload.instanceID =
        gRootConstant.baseInstanceIndex + tid.x;

    // 1×1×1 の MeshShader グループを発行
    DispatchMesh(uint3(1, 1, 1), payload);
}