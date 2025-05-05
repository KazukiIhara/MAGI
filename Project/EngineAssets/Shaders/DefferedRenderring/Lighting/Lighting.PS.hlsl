#include "Lighting.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);

Texture2D gAlbedoTex : register(t0);
Texture2D gNormalTex : register(t1);
Texture2D gPositionTex : register(t2);

ConstantBuffer<DirectionalLightData> gDirectionalLight : register(b1);

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
    
    // 元の色とalphaを取得
    float3 baseColor = albedo.rgb;
    float alpha = albedo.a;
    
    // ライトによる加算用の値
    float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);

    // Directional Light
    const float3 directionalLightDirection = normalize(gDirectionalLight.direction);
    const float directionalLightIntensity = gDirectionalLight.intencity;
    const float3 directionalLightColor = gDirectionalLight.color; 
   
    //
    // DirectionalLight
    //
    {
        float3 L = -normalize(directionalLightDirection);
        float NdotL = saturate(dot(normal, L));
        float3 diffuse = directionalLightColor * directionalLightIntensity * NdotL;
        totalDiffuse += diffuse;
    }
    
    output.color.rgb = baseColor * totalDiffuse;
    output.color.a = alpha;
    
    return output;
}
