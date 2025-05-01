#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
StructuredBuffer<ModelDataForGPU> gInstanceData : register(t0);
StructuredBuffer<VertexData3D> gVertexData : register(t1);
StructuredBuffer<uint> gIndexData : register(t2);

[outputtopology("triangle")]
[numthreads(1, 1, 1)]
void main(in payload ASPayload payload,
          uint3 tid : SV_DispatchThreadID,
          out indices uint3 tris[1],
          out vertices MeshOutput verts[3])
{
    uint triID = tid.x;
    uint base = payload.firstIndex + triID * 3;
    uint3 idx = uint3(gIndexData[base + 0],
                       gIndexData[base + 1],
                       gIndexData[base + 2]);

    VertexData3D v[3] =
    {
        gVertexData[idx.x],
        gVertexData[idx.y],
        gVertexData[idx.z]
    };

    SetMeshOutputCounts(3, 1);

    [unroll]
    for (uint i = 0; i < 3; ++i)
    {
        float4 wp = mul(v[i].position, gInstanceData[payload.instanceID].worldMatrix);
        verts[i].position = mul(wp, gCamera.viewProjection);
        verts[i].uv = v[i].uv;
        verts[i].instanceIndex = payload.instanceID;
    }
    tris[0] = uint3(0, 1, 2);
}