// ── Mesh Shader ─────────────────────────────────────
#include "Triangle3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<TriangleData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload, // ← 受け取り
          uint3 tid : SV_DispatchThreadID,
          out indices uint3 tris[1],
          out vertices MeshOutput verts[3])
{
    const uint instanceID = payload.instanceID;
    
    // デバッグ用
    const float marker = payload.markerValue;

    TriangleData3D data = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    SetMeshOutputCounts(3, 1);

    // 3 頂点ぶん処理
    [unroll]
    for (uint i = 0; i < 3; ++i)
    {
        float4 localPos = float4(data.vertices[i].xyz, 1.0f);
        float4 worldPos = mul(localPos, data.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);

        float2 baseUV = float2(data.vertices[i].x + 0.5f,
                               1.0f - (data.vertices[i].y + 0.5f));
        float2 uv = mul(float4(baseUV, 0.0f, 1.0f), mat.uvMatrix).xy;

        verts[i].position = clipPos;
        verts[i].uv = uv;
        verts[i].instanceIndex = instanceID;
    }

    tris[0] = uint3(0, 1, 2);
}
