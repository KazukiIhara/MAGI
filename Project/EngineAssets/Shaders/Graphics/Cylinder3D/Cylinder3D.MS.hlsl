#include "Cylinder3D.hlsli"

#define MAX_VERTICES 4
#define MAX_TRIANGLES 2
#define PI 3.14159265f

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<CylinderData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    in payload ASPayload payload,
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices MeshOutput verts[MAX_VERTICES]
)
{
    uint instanceID = payload.instanceID;
    uint tileID = threadID.x;

    CylinderData3D cylinder = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];
    
    uint divide = cylinder.cylinderDivide;
    
    bool isActive = (tileID < divide);
    SetMeshOutputCounts(isActive ? 4 : 0, isActive ? 2 : 0);
    if (!isActive)
        return;
    
    float radianParDivide = (2.0f * PI) / divide;
    
    float angle0 = tileID * radianParDivide;
    float angle1 = (tileID + 1) * radianParDivide;

    float cos0 = cos(angle0);
    float sin0 = sin(angle0);
    float cos1 = cos(angle1);
    float sin1 = sin(angle1);
    
    // XY平面でのシリンダー頂点
    float3 p0 = float3(cos0 * cylinder.topRadius, cylinder.height, sin0 * cylinder.topRadius); // 外側開始
    float3 p1 = float3(cos1 * cylinder.topRadius, cylinder.height, sin1 * cylinder.topRadius); // 外側終了
    float3 p2 = float3(cos0 * cylinder.bottomRadius, 0.0f, sin0 * cylinder.bottomRadius); // 内側開始
    float3 p3 = float3(cos1 * cylinder.bottomRadius, 0.0f, sin1 * cylinder.bottomRadius); // 内側終了

    float4 positions[4] =
    {
        mul(float4(p0, 1.0f), cylinder.worldMatrix),
        mul(float4(p1, 1.0f), cylinder.worldMatrix),
        mul(float4(p2, 1.0f), cylinder.worldMatrix),
        mul(float4(p3, 1.0f), cylinder.worldMatrix)
    };

    float u0 = (float) tileID / (float) divide;
    float u1 = (float) (tileID + 1) / (float) divide;

    float2 baseUVs[4] =
    {
        float2(u0, 0.0f),
        float2(u1, 0.0f),
        float2(u0, 1.0f),
        float2(u1, 1.0f)
    };

    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        float4 clipPos = mul(positions[i], gCamera.viewProjection);
        float4 transformedUV = mul(float4(baseUVs[i], 0.0f, 1.0f), mat.uvMatrix);

        verts[i].position = clipPos;
        verts[i].uv = transformedUV.xy;
        verts[i].instanceIndex = instanceID;
    }


    tris[0] = uint3(0, 2, 1);
    tris[1] = uint3(2, 3, 1);
}