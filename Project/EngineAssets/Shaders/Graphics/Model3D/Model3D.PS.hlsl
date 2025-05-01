#include "Model3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
ConstantBuffer<PrimitiveMaterialData3D> gMaterialData : register(b1);

float4 main(MeshOutput input) : SV_Target
{
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData;

    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, input.uv);
    return texColor * mat.baseColor;
}