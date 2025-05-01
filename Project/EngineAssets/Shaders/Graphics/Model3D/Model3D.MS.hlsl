#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<ModelDataForGPU> gInstanceData : register(t0);
StructuredBuffer<VertexData3D> gVertexData : register(t1);
StructuredBuffer<uint> gIndexData : register(t2);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[1],
    out vertices MeshOutput verts[3]
)
{
    uint instanceID = payload.instanceID;

    ModelDataForGPU modelData = gInstanceData[instanceID];

    SetMeshOutputCounts(3, 1); // 3 vertices, 1 triangle

    // インデックスから頂点を取得
    uint index0 = gIndexData[0];
    uint index1 = gIndexData[1];
    uint index2 = gIndexData[2];

    VertexData3D v0 = gVertexData[index0];
    VertexData3D v1 = gVertexData[index1];
    VertexData3D v2 = gVertexData[index2];

    // それぞれの頂点について変換
    verts[0].position = mul(mul(float4(v0.position, 1.0f), modelData.worldMatrix), gCamera.viewProjection);
    verts[0].uv = v0.uv;
    verts[0].instanceIndex = instanceID;

    verts[1].position = mul(mul(float4(v1.position, 1.0f), modelData.worldMatrix), gCamera.viewProjection);
    verts[1].uv = v1.uv;
    verts[1].instanceIndex = instanceID;

    verts[2].position = mul(mul(float4(v2.position, 1.0f), modelData.worldMatrix), gCamera.viewProjection);
    verts[2].uv = v2.uv;
    verts[2].instanceIndex = instanceID;

    // 三角形構成
    tris[0] = uint3(0, 1, 2);
}
