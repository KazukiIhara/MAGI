#include "Lighting.hlsli"

//================================ 
// CBV
//================================
ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<DirectionalLightData> gDirectionalLight : register(b1);
ConstantBuffer<LightCameraData> gLightCamera : register(b2);

//================================ 
// GBuffer
//================================
Texture2D<float4> gAlbedoTex : register(t0);
Texture2D<float4> gNormalTex : register(t1);
Texture2D<float4> gPositionTex : register(t2);

//================================ 
// ShadowMapTex
//================================
Texture2D<float> gShadowMap : register(t3);

//================================
// EnvironmentTex
//================================
TextureCube<float4> gEnvironmentTex : register(t4);

//================================
// Samplers
//================================
SamplerState gSampler : register(s0);
SamplerComparisonState gShadowSampler : register(s1);

//================================
// HelperFuncs
//================================

float3 ComputeDirectionalLight(float3 normal)
{
    const float3 directionalLightDirection = normalize(gDirectionalLight.direction);
    const float directionalLightIntensity = gDirectionalLight.intencity;
    const float3 directionalLightColor = gDirectionalLight.color;
    float3 L = -directionalLightDirection;
    float NdotL = saturate(dot(normal, L));
    
    return directionalLightColor * directionalLightIntensity * NdotL;
}

float ComputeDirectionalLightShadow(float3 worldPos, float3 worldNormal)
{
    // ライト空間投影
    float4 lsPos = mul(float4(worldPos, 1.0f), gLightCamera.viewProjection);
    lsPos /= lsPos.w;

    // UV 変換（NDC→[0,1]）
    float2 uv = lsPos.xy * float2(0.5f, -0.5f) + 0.5f;

    // 法線ベースの可変バイアス計算
    float3 N = normalize(worldNormal);
    float3 L = normalize(-gDirectionalLight.direction);
    float ndl = saturate(dot(N, L)); // [0,1]
    float normalBias = ConstantBias + (1.0f - ndl) * SlopeBiasScale;

    // 増減バイアスを反映した深度参照値
    float depthRef = lsPos.z - normalBias;

    // シャドウテスト
    float shadow = 1.0f;
    if (uv.x >= 0.0f && uv.x <= 1.0f &&
        uv.y >= 0.0f && uv.y <= 1.0f)
    {
        // SampleCmpLevelZero で比較
        shadow = gShadowMap.SampleCmpLevelZero(gShadowSampler, uv, depthRef);
    }
    return shadow;
}

float3 ComputeEnvironment(float3 position, float3 normal, float coefficient)
{
    float3 cameraToPosition = normalize(position - gCamera.worldPosition);
    float3 reflectedVector = reflect(cameraToPosition, normalize(normal));
    float3 environmentColor = gEnvironmentTex.Sample(gSampler, reflectedVector).rgb;
    
    return environmentColor * coefficient;
}

//================================
// EntryPoint
//================================
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // GBufferから取得
    float4 albedo = gAlbedoTex.Sample(gSampler, input.texcoord);
    float4 normalRaw = gNormalTex.Sample(gSampler, input.texcoord);
    float4 position = gPositionTex.Sample(gSampler, input.texcoord);

    // Normalを[-1,1]空間に戻して正規化
    float3 normal = normalize(normalRaw.xyz * 2.0f - 1.0f);
    
    // 元の色とalphaを取得
    float3 baseColor = albedo.rgb;
    float alpha = albedo.a;
    
    // カラー加算用の値
    float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);
     
    // 
    // DirectionalLight
    // 
    {      
        float3 directionalLightDiffuse = ComputeDirectionalLight(normal);
        float directionalLightShadow = ComputeDirectionalLightShadow(position.xyz, normal);
        totalDiffuse += directionalLightDiffuse * directionalLightShadow;
    }
    
    //
    // Environment
    //
    {
        float3 environmentDiffuse = ComputeEnvironment(position.xyz, normal, 0.04f);
        totalDiffuse += environmentDiffuse;
    }
     
    // 
    // FinalColor
    // 
    output.color.rgb = baseColor * totalDiffuse;
    output.color.a = alpha;
    
    return output;
}
