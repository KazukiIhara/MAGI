#include "Model3D.hlsli"

ConstantBuffer<RootConstants> gRootConst : register(b2);
StructuredBuffer<Meshlet> gMeshlets : register(t3);

[numthreads(1, 1, 1)]
void main(uint3 gid : SV_GroupID)
{
    uint meshletID = gid.x;
    uint instanceID = gRootConst.baseInstanceIndex + gid.y;

    if (meshletID >= gMeshlets.Length)
        return;

    ASPayload p;
    p.instanceID = instanceID;
    p.meshletID = meshletID;

    DispatchMesh(1, 1, 1, p);
}
