#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<ModelDataForGPU> gInstance : register(t0);
StructuredBuffer<VertexData3D> gVertices : register(t1);
StructuredBuffer<uint> gUniqueVert : register(t4);
StructuredBuffer<PackedTriangle> gPrimIB : register(t5);
StructuredBuffer<Meshlet> gMeshlets : register(t3);

#define MAX_VERTS 64
#define MAX_PRIMS 126

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload,
          out indices uint3 tris[MAX_PRIMS],
          out vertices MeshOutput verts[MAX_VERTS])
{
    Meshlet ml = gMeshlets[payload.meshletID];

    SetMeshOutputCounts(ml.vertCount, ml.primCount);

    // ── 頂点展開 ──
    [loop]
    for (uint i = 0; i < ml.vertCount; ++i)
    {
        uint vertIdx = gUniqueVert[ml.vertOffset + i];
        VertexData3D v = gVertices[vertIdx];

        float4 wp = mul(v.position, gInstance[payload.instanceID].worldMatrix);
        verts[i].position = mul(wp, gCamera.viewProjection);
        verts[i].uv = v.uv;
        verts[i].instanceIndex = payload.instanceID;
    }

    // ── 三角形書き込み ──
    [loop]
    for (uint i = 0; i < ml.primCount; ++i)
    {
        uint packed = gPrimIB[ml.primOffset + i];
        uint3 tri;
        tri.x = packed & 0x3FF; // 0-9bit
        tri.y = (packed >> 10) & 0x3FF; // 10-19bit
        tri.z = (packed >> 20) & 0x3FF; // 20-29bit
        tris[i] = tri;
    }
}
