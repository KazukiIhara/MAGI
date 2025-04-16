#include "Sphere3D.hlsli"

StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);
Texture2D gTextures[]; // Bindless Texture
SamplerState gSampler : register(s0);

PixelShaderOutput main(GeometryShaderOutput input)
{
    PixelShaderOutput output;

    PrimitiveMaterialData3D mat = gMaterialData[input.instanceID];

    float4 color = mat.baseColor;

    // UVトランスフォーム
    float2 uv = input.texcoord - 0.5f;
    float c = cos(mat.uvRotation);
    float s = sin(mat.uvRotation);
    uv = float2(c * uv.x - s * uv.y, s * uv.x + c * uv.y);
    uv *= mat.uvScale;
    uv += mat.uvTransform + 0.5f;

    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    color *= texColor;

    output.color = color;
    return output;
}