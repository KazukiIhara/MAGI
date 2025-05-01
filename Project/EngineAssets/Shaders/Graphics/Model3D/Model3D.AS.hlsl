#include "Model3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b2);
StructuredBuffer<DrawMeshlet> gMeshlets : register(t3);

[numthreads(1, 1, 1)]
void main(uint3 gid : SV_GroupID)
{
    uint meshletID = gid.x;
    uint instanceID = gRootConstant.baseInstanceIndex + gid.y;

    uint elemCount, stride;
    gMeshlets.GetDimensions(elemCount, stride);

    // 範囲外アクセスを防ぎつつ、どのパスでも DispatchMesh を呼ぶ
    DrawMeshlet ml;
    if (meshletID < elemCount)
    {
        ml = gMeshlets[meshletID];
    }
    else
    {
        // ダミー (0 プリミティブ) を用意
        ml.firstIndex = 0;
        ml.primitiveCount = 0;
    }

    ASPayload payload;
    payload.instanceID = instanceID;
    payload.firstIndex = ml.firstIndex; // 0 でも OK

    // ここが **全コードパス共通** で 1 回だけ実行される
    DispatchMesh(ml.primitiveCount, 1, 1, payload);
}