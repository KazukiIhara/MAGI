#include "Model3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b2);
StructuredBuffer<uint> gIndexData : register(t2);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    payload.instanceID = gRootConstant.baseInstanceIndex + tid.y;
    
    uint indexCount;
    uint stride;
    
    gIndexData.GetDimensions(indexCount, stride); // stride = sizeof(uint)
    uint triCount = indexCount / 3;
    
    DispatchMesh(triCount, 1, 1, payload);
}
