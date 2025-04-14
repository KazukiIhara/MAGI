#include "Plane3D.hlsli"

// PS用のBindlessアクセス設定
StructuredBuffer<PlaneMaterialData3D> gMaterialData : register(t2);
Texture2D gTextures[]; // Bindlessなテクスチャ配列（Shader Model 6.6）
SamplerState gSampler : register(s0);

PixelShaderOutput main(GeometryShaderOutput input)
{
    PixelShaderOutput output;

    uint texIndex = gMaterialData[input.instanceID].textureIndex;
    float4 texColor = gTextures[texIndex].Sample(gSampler, input.texcoord);

    output.color = texColor * input.color; // 頂点カラーと乗算
    return output;
}