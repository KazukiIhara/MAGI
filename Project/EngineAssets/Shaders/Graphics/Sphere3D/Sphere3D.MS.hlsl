#include "Sphere3D.hlsli"

#define MAX_VERTICES 4
#define MAX_TRIANGLES 2

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<SphereData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload,
    uint3 threadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIANGLES],
    out vertices MeshOutput verts[MAX_VERTICES]
)
{
    uint instanceID = payload.instanceID;
    uint tileID = threadID.x;

    SphereData3D sphere = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    const uint lonCount = sphere.longitudeSegments;
    const uint latCount = sphere.latitudeSegments;
    const uint totalTiles = lonCount * latCount;

    bool isActive = (tileID < totalTiles);
    SetMeshOutputCounts(isActive ? 4 : 0, isActive ? 2 : 0);
    if (!isActive)
        return;

    const uint lonIndex = tileID % lonCount;
    const uint latIndex = tileID / lonCount;

    const float PI = 3.14159265f;
    const float radius = sphere.radius;
    const float lonStep = 2.0f * PI / lonCount;
    const float latStep = PI / latCount;

    float theta0 = -PI / 2.0f + latIndex * latStep;
    float theta1 = -PI / 2.0f + (latIndex + 1) * latStep;
    float phi0 = lonIndex * lonStep;
    float phi1 = (lonIndex + 1) * lonStep;

    float3 positions[4] =
    {
        float3(cos(theta0) * cos(phi0), sin(theta0), cos(theta0) * sin(phi0)),
        float3(cos(theta1) * cos(phi0), sin(theta1), cos(theta1) * sin(phi0)),
        float3(cos(theta0) * cos(phi1), sin(theta0), cos(theta0) * sin(phi1)),
        float3(cos(theta1) * cos(phi1), sin(theta1), cos(theta1) * sin(phi1))
    };

    float2 uvs[4] =
    {
        float2((float) lonIndex / lonCount, 1.0f - (float) latIndex / latCount),
        float2((float) lonIndex / lonCount, 1.0f - (float) (latIndex + 1) / latCount),
        float2((float) (lonIndex + 1) / lonCount, 1.0f - (float) latIndex / latCount),
        float2((float) (lonIndex + 1) / lonCount, 1.0f - (float) (latIndex + 1) / latCount)
    };

    for (uint i = 0; i < 4; ++i)
    {
        float4 worldPos = mul(float4(positions[i] * radius, 1.0f), sphere.worldMatrix);
        float4 clipPos = mul(worldPos, gCamera.viewProjection);
        float4 uvTransformed = mul(float4(uvs[i], 0.0f, 1.0f), mat.uvMatrix);

        verts[i].position = clipPos;
        verts[i].uv = uvTransformed.xy;
        verts[i].color = mat.baseColor;
        verts[i].instanceIndex = instanceID;
    }

    tris[0] = uint3(0, 1, 2);
    tris[1] = uint3(2, 1, 3);
}
