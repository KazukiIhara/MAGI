#include "Model3D.hlsli"

ConstantBuffer<RootConstants> gRootConstant : register(b2);

[numthreads(1, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID)
{
    ASPayload payload;
    payload.instanceID = gRootConstant.baseInstanceIndex + tid.y;

    DispatchMesh(1, 1, 1, payload);
}
