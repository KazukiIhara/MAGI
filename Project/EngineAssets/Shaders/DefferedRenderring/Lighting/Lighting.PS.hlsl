#include "Lighting.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);

Texture2D gAlbedoTex : register(t0);
Texture2D gNormalTex : register(t1);
Texture2D gPositionTex : register(t2);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // GBufferから取得
    float4 albedo = gAlbedoTex.Sample(gSampler, input.texcoord);
    float4 normalRaw = gNormalTex.Sample(gSampler, input.texcoord);
    float4 position = gPositionTex.Sample(gSampler, input.texcoord);

    // Normalを[-1,1]空間に戻して正規化（ワールド空間想定）
    float3 normal = normalize(normalRaw.xyz * 2.0f - 1.0f);

    // ----------- 固定ライト情報 ------------

    // Directional Light
    const float3 directionalLightDirection = normalize(float3(0.0f, -1.0f, 0.0f)); // 真上から
    const float3 directionalLightColor = float3(1.0f, 1.0f, 1.0f); // 白色
    const float directionalLightIntensity = 0.5f;

    // ----------------------------------------

    float3 baseColor = albedo.rgb;
    float alpha = albedo.a;
    
    float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);

    //
    // DirectionalLight contribution
    //
    {
        float3 L = -normalize(directionalLightDirection); // ライトの逆方向
        float NdotL = saturate(dot(normal, L));
        float3 diffuse = directionalLightColor * directionalLightIntensity * NdotL;
        totalDiffuse += diffuse;
    }
    
    output.color.rgb = baseColor * totalDiffuse;
    output.color.a = alpha;
    
    return output;
}
