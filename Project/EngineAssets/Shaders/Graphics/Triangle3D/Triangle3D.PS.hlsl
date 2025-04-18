#include "Triangle3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

float4 main(MeshOutput input) : SV_Target
{
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    float4 uvTransformed = mul(float4(input.uv, 0.0f, 1.0f), mat.uvMatrix);
    float2 uv = uvTransformed.xy;

    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    return texColor * mat.baseColor;
}