#include "Triangle3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b1);
StructuredBuffer<TriangleData3D> gInstanceData : register(t0);

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint instanceID = gRootConstant.baseInstanceIndex + dispatchThreadID.x;

    // Dispatch with payload = instanceID
    DispatchMesh(uint3(1, 1, 1), instanceID); // payload
}