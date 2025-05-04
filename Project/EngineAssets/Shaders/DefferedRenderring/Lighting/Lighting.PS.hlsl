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

    // normalを[0,1]→[-1,1]に戻す
    float3 normal = normalize(normalRaw.xyz * 2.0f - 1.0f);

    // ----------- 固定ライト情報（ここ直書き） ------------
    float3 lightDirection = normalize(float3(0.0f, -1.0f, 0.0f)); // 真上から
    float3 lightColor = float3(1.0f, 1.0f, 1.0f); // 白ライト
    //------------------------------------------------------

    // ランバート反射
    float3 L = normalize(-lightDirection);
    float NdotL = saturate(dot(normal, L));
    float3 diffuse = albedo.rgb * lightColor * NdotL;

    output.color = float4(diffuse, 1.0f);
    
    return output;
}