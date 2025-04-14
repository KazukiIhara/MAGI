#include "Plane3D.hlsli"

StructuredBuffer<PlaneData3D> gInstanceData : register(t0);

VertexShaderOutput main(VertexShaderInput input, uint instanceID : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = float4(0.0f, 0.0f, 0.0f, 1.0f); // É_É~Å[
    output.texcoord = float2(0.0f, 0.0f);
    output.normal = float3(0.0f, 1.0f, 0.0f);
    output.tangent = float3(1.0f, 0.0f, 0.0f);
    output.worldPosition = float3(0.0f, 0.0f, 0.0f);
    output.instanceID = instanceID;
    return output;
}