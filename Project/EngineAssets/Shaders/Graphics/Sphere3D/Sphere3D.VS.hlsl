#include "Sphere3D.hlsli"

StructuredBuffer<SphereData3D> gInstanceData : register(t0);

VertexShaderOutput main(uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = float4(0.0f, 0.0f, 0.0f, 1.0f); // É_É~Å[
    output.instanceID = instanceID;
    return output;
}
