#include "Lighting.hlsli"

// Lighting.hlsli

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<DirectionalLightData> gDirectionalLight : register(b1);
ConstantBuffer<LightCameraData> gLightCamera : register(b2);

Texture2D gAlbedoTex : register(t0);
Texture2D gNormalTex : register(t1);
Texture2D gPositionTex : register(t2);
Texture2D<float> gShadowMap : register(t3);

SamplerState gSampler : register(s0);

float ComputeShadow(float3 worldPos)
{
    float shadowMap = 0.0f;
    
    float4 lsPos = mul(float4(worldPos, 1), gLightCamera.viewProjection);
    
    // ライトビュースクリーン空間からUV空間に座標変換
    float2 shadowMapuv = lsPos.xy / lsPos.w;
    shadowMapuv *= float2(0.5f, -0.5f);
    shadowMapuv += 0.5f;
    
    // ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = lsPos.z / lsPos.w;
    
    if (shadowMapuv.x > 0.0f && shadowMapuv.x < 1.0f
        && shadowMapuv.y > 0.0f && shadowMapuv.y < 1.0f)
    {
        // シャドウマップに描き込まれているZ値と比較する
        float zInShadowMap = gShadowMap.Sample(gSampler, shadowMapuv).r;
        if (zInLVP <= zInShadowMap)
        {
            shadowMap = 1.0f;
        }
    }
    return shadowMap;
}

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
   
    float3 worldPos = position.xyz;

    // シャドウ係数を取得
    float shadow = ComputeShadow(worldPos);
    
    //
    // DirectionalLight
    //
    {
        float3 L = -directionalLightDirection;
        float NdotL = saturate(dot(normal, L));
        float3 diffuse = directionalLightColor * directionalLightIntensity * NdotL * shadow;
        totalDiffuse += diffuse;
    }
    
    output.color.rgb = baseColor * totalDiffuse;
    output.color.a = alpha;
    
    return output;
}
