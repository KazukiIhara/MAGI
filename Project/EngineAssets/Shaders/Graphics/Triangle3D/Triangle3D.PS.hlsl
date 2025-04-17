#include "Triangle3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

float4 main(MeshOutput input) : SV_Target
{
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    float2 uv = input.uv * mat.uvScale + mat.uvTranslate;

    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    return texColor * mat.baseColor;
}