#include "Sphere3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b1);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    payload.instanceID = gRootConstant.baseInstanceIndex + tid.y;

    DispatchMesh(256, 1, 1, payload);
}
