#include "Sphere3D.hlsli"

#define MAX_VERTS 256
#define MAX_TRIS 256

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<SphereData3D> gInstanceData : register(t0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(
    uint3 dispatchThreadID : SV_DispatchThreadID,
    out indices uint3 tris[MAX_TRIS],
    out vertices MeshOutput verts[MAX_VERTS]
)
{
    uint instanceID = dispatchThreadID.y;
    SphereData3D sphere = gInstanceData[instanceID];
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    uint lat = sphere.latitudeSegments;
    uint lon = sphere.longitudeSegments;

    uint vertexCount = (lat + 1) * (lon + 1);
    uint indexCount = lat * lon * 6;

    SetMeshOutputCounts(vertexCount, indexCount / 3);

    uint index = 0;
    for (uint y = 0; y <= lat; ++y)
    {
        float v = (float) y / lat;
        float theta = v * 3.14159265;

        for (uint x = 0; x <= lon; ++x)
        {
            float u = (float) x / lon;
            float phi = u * 2.0f * 3.14159265;

            float3 localPos = float3(
                sin(theta) * cos(phi),
                cos(theta),
                sin(theta) * sin(phi)
            ) * sphere.radius;

            float4 worldPos = mul(float4(localPos, 1.0f), sphere.worldMatrix);
            float4 clipPos = mul(worldPos, gCamera.viewProjection);

            float2 baseUV = float2(u, 1.0f - v);
            float4 transformedUV = mul(float4(baseUV, 0.0f, 1.0f), mat.uvMatrix);

            verts[index].position = clipPos;
            verts[index].uv = transformedUV.xy;
            verts[index].color = mat.baseColor;
            verts[index].instanceIndex = instanceID;
            ++index;
        }
    }

    index = 0;
    for (uint y = 0; y < lat; ++y)
    {
        for (uint x = 0; x < lon; ++x)
        {
            uint i0 = y * (lon + 1) + x;
            uint i1 = i0 + 1;
            uint i2 = i0 + lon + 1;
            uint i3 = i2 + 1;

            tris[index++] = uint3(i0, i2, i1);
            tris[index++] = uint3(i1, i2, i3);
        }
    }
}
