#include "Plane3D.hlsli"

// PS用のBindlessアクセス設定
StructuredBuffer<PlaneMaterialData3D> gMaterialData : register(t1);
Texture2D gTextures[]; // Bindlessなテクスチャ配列（Shader Model 6.6）
SamplerState gSampler : register(s0);

PixelShaderOutput main(GeometryShaderOutput input)
{
    PixelShaderOutput output;

    PlaneMaterialData3D mat = gMaterialData[input.instanceID];

    float4 color = mat.baseColor;
    
    // UVトランスフォーム
    float2 uv = input.texcoord - 0.5f;
    float c = cos(mat.uvRotate);
    float s = sin(mat.uvRotate);
    uv = float2(c * uv.x - s * uv.y, s * uv.x + c * uv.y); // 回転
    uv *= mat.uvScale;
    uv += mat.uvTranslate + 0.5f;
    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    color *= texColor;


    output.color = color;
    
    return output;
}