#include "Model3D.hlsli"

// b2 に入っている MeshInfo をそのまま利用
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);

// -----------------------------------------------------------------------------
// ひとまず「CPU 側は DispatchMesh(1,1,1) を呼ぶ」前提。
// この AS が gMeshInfo.MeshletCount 個の MS スレッドグループを生成するだけ。
[shader("amplification")]
[numthreads(AS_GROUP_SIZE, 1, 1)]
void main(uint3 dtid : SV_DispatchThreadID)
{
    bool visible = true;
   
    Payload payload;
    payload.instanceID = dtid.y;
    
    uint index = WavePrefixCountBits(visible);
    payload.meshletIndices[index] = dtid.x;

    // MS へは meshletCount × 1 × 1 TG を飛ばす
    DispatchMesh(gMeshInfo.MeshletCount, 1, 1, payload);
}
