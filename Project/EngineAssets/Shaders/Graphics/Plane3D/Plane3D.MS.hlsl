#include "Plane3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<PlaneData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[2],
    out vertices MeshOutput verts[6]
)
{
    uint instanceID = dispatchThreadID.y;
    PlaneData3D plane = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    SetMeshOutputCounts(6, 2); // 2 triangles from 4 vertices

    float4 positions[4] =
    {
        plane.offsets[0], // LT
        plane.offsets[1], // RT
        plane.offsets[2], // LB
        plane.offsets[3] // RB
    };

    float2 uvs[4] =
    {
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f)
    };

    // ç¿ïWÇ∆UVÇê›íË
    for (uint i = 0; i < 4; ++i)
    {
        float4 worldPos = mul(positions[i], plane.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);

        float4 transformedUV = mul(float4(uvs[i], 0.0f, 1.0f), mat.uvMatrix);

        verts[i].position = clipPos;
        verts[i].uv = transformedUV.xy;
        verts[i].color = mat.baseColor;
        verts[i].instanceIndex = instanceID;
    }

    // Triangle indices
    tris[0] = uint3(0, 1, 2); // Triangle 1
    tris[1] = uint3(2, 1, 3); // Triangle 2
}
