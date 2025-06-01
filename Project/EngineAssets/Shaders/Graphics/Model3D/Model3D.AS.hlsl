#include "Model3D.hlsli"

ConstantBuffer<MeshInfo> gMeshInfo : register(b2);

[shader("amplification")]
[numthreads(AS_GROUP_SIZE, 1, 1)]
void main(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID)
{   
    uint meshletID = gid.x * AS_GROUP_SIZE + dtid.x;
    
    bool visible = false;
   
    Payload payload;
    payload.instanceID = dtid.y;
    
    if (dtid.x < gMeshInfo.MeshletCount)
    {
        visible = true;
    }
    
    if (visible)
    {
        uint index = WavePrefixCountBits(visible);
        payload.meshletIndices[index] = meshletID;
    }
    
    uint visibleCount = WaveActiveCountBits(visible);

    DispatchMesh(visibleCount, 1, 1, payload);
}
