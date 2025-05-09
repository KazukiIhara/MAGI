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
SamplerComparisonState gShadowSampler : register(s1);

static const float ShadowBias = 0.002f;
static const int ShadowMapSize = 2048;

float ComputeShadow(float3 worldPos)
{
    // ライト空間へ変換・透視除算
    float4 lsPos = mul(gLightCamera.lightViewProjection, float4(worldPos, 1.0f));
    lsPos /= lsPos.w;

    // UV は [&-1,1]→[0,1] の変換のみ
    float2 uv = lsPos.xy * 0.5f + 0.5f;

    // 深度バイアス
    float depth = lsPos.z + ShadowBias;

    // 射影範囲外はシャドウなし（バウンダリーサンプラーを border=1 にしていれば省略可）
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        return 1.0f;

    // ハードウェア PCF（サンプラーを COMPARISON_MIN_MAG_MIP_LINEAR に設定しておく）
    return gShadowMap.SampleCmpLevelZero(gShadowSampler, uv, depth);
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

     // 1) G-Buffer 読み出し
    float4 albedoRaw = gAlbedoTex.Sample(gSampler, input.texcoord);
    float4 normalRaw = gNormalTex.Sample(gSampler, input.texcoord);
    float4 position = gPositionTex.Sample(gSampler, input.texcoord);

    float3 baseColor = albedoRaw.rgb;
    float alpha = albedoRaw.a;
    float3 normal = normalize(normalRaw.xyz * 2.0f - 1.0f);

    // 2) ライト情報
    float3 Ldir = normalize(gDirectionalLight.direction);
    float lint = gDirectionalLight.intencity;
    float3 lcol = gDirectionalLight.color;

    // 3) N·L チェック
    float NdotL = dot(normal, -Ldir);
    if (NdotL <= 0.0f)
    {
        // 裏面、もしくはライト背面は影もライトもなし
        output.color = float4(0, 0, 0, alpha);
        return output;
    }

    // 4) シャドウ
    float shadow = ComputeShadow(position.xyz);

    // 5) ライティング
    float3 diffuse = lcol * lint * NdotL * shadow;
    float3 lit = baseColor * diffuse;

    output.color = float4(lit, alpha);
    return output;
}
