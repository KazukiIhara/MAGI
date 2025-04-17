#include "Triangle3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<TriangleData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[1],
    out vertices MeshOutput verts[3]
)
{
    uint instanceID = dispatchThreadID.y;
    TriangleData3D data = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    SetMeshOutputCounts(3, 1);

    float3 positions[3] =
    {
        float3(-0.5f, -0.5f, 0.0f),
        float3(0.0f, 0.5f, 0.0f),
        float3(0.5f, -0.5f, 0.0f)
    };

    for (uint i = 0; i < 3; ++i)
    {
        float4 localPos = float4(positions[i], 1.0f);
        float4 worldPos = mul(localPos, data.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);

        verts[i].position = clipPos;
        verts[i].uv = float2(positions[i].x + 0.5f, 1.0f - (positions[i].y + 0.5f)); // 0~1‚Ö
        verts[i].color = mat.baseColor;
        verts[i].instanceIndex = instanceID;
    }

    tris[0] = uint3(0, 1, 2);
}
