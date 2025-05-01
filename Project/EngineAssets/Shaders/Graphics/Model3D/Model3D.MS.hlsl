#include "Model3D.hlsli"

// ------ RootSignature ------
// b0:Camera  b1:Material
// t0:InstanceData  t1:Vertex  t3:Meshlet  t4:Unique  t5:Tri


ConstantBuffer<Camera> gCam : register(b0);
ConstantBuffer<PrimitiveMaterialData3D> gMat : register(b1);
StructuredBuffer<ModelDataForGPU> gInst : register(t0);
StructuredBuffer<VertexData3D> gVtx : register(t1);
StructuredBuffer<Meshlet> gML : register(t2);
StructuredBuffer<uint> gUNI : register(t3);
StructuredBuffer<PackedTriangle> gTRI : register(t4);

[outputtopology("triangle")]
[numthreads(128, 1, 1)]
void main(uint3 gid : SV_GroupID, uint lid : SV_GroupIndex,
          out vertices MeshOutput vOut[128],
          out indices uint3 idx[128])
{
    Meshlet ml = gML[gid.x]; // x=MeshletID, y=InstanceID

    // 必ず 1 回
    SetMeshOutputCounts(ml.vertCount, ml.primCount);

    // ----- 三角形 -----
    if (lid < ml.primCount)
    {
        uint p = gTRI[ml.primOffset + lid];
        idx[lid] = uint3(p & 0x3FF, (p >> 10) & 0x3FF, (p >> 20) & 0x3FF);
    }

    // ----- 頂点 -----
    if (lid < ml.vertCount)
    {
        uint vIdx = gUNI[ml.vertOffset + lid];
        VertexData3D v = gVtx[vIdx];

        float4 wp = mul(v.pos, gInst[gid.y].world); // gid.y = instance
        vOut[lid].position = mul(wp, gCam.vp);
        vOut[lid].uv = v.uv;
        vOut[lid].instID = gid.y;
    }
}
