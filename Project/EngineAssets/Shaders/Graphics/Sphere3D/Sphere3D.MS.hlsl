#include "Sphere3D.hlsli"

#define MAX_VERTICES 256
#define MAX_TRIANGLES 256

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<SphereData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices MeshOutput verts[MAX_VERTICES]
)
{
    uint instanceID = threadID.y;
    SphereData3D sphere = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    const uint lonCount = min(sphere.longitudeSegments, 15);
    const uint latCount = min(sphere.latitudeSegments, 15);
    const float radius = sphere.radius;

    const uint vertexCount = (latCount + 1) * (lonCount + 1);
    const uint triangleCount = latCount * lonCount * 2;

    // 🔴 ここで必ず最初に呼び出す
    SetMeshOutputCounts(vertexCount, triangleCount);

    const float PI = 3.14159265f;
    const float lonStep = 2.0f * PI / lonCount;
    const float latStep = PI / latCount;

    uint vertexIndex = 0;
    uint triangleIndex = 0;

    // 頂点生成
    for (uint latIndex = 0; latIndex <= latCount; ++latIndex)
    {
        float theta = -PI / 2.0f + latIndex * latStep;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (uint lonIndex = 0; lonIndex <= lonCount; ++lonIndex)
        {
            float phi = lonIndex * lonStep;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float3 localPos = float3(
                cosTheta * cosPhi,
                sinTheta,
                cosTheta * sinPhi
            );

            float4 worldPos = mul(float4(localPos * radius, 1.0f), sphere.worldMatrix);
            float4 clipPos = mul(worldPos, gCamera.viewProjection);

            float2 uv = float2((float) lonIndex / lonCount, 1.0f - (float) latIndex / latCount);
            float4 uvTransformed = mul(float4(uv, 0.0f, 1.0f), mat.uvMatrix);

            verts[vertexIndex].position = clipPos;
            verts[vertexIndex].uv = uvTransformed.xy;
            verts[vertexIndex].color = mat.baseColor;
            verts[vertexIndex].instanceIndex = instanceID;

            vertexIndex++;
        }
    }

    // 三角形生成
    for (uint latIndex = 0; latIndex < latCount; ++latIndex)
    {
        for (uint lonIndex = 0; lonIndex < lonCount; ++lonIndex)
        {
            uint v0 = latIndex * (lonCount + 1) + lonIndex;
            uint v1 = (latIndex + 1) * (lonCount + 1) + lonIndex;
            uint v2 = v0 + 1;
            uint v3 = v1 + 1;

            tris[triangleIndex++] = uint3(v0, v1, v2);
            tris[triangleIndex++] = uint3(v1, v3, v2);
        }
    }
}
