#include "RadialBlur.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<RadialBlurData> gdata : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    const float2 center = gdata.param0.xy;
    const uint numSamples = 10;
    const float2 blurWidth = gdata.param1.x;
    
    float2 direction = input.texcoord - center;
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (uint sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        float2 texcoord = input.texcoord + direction * blurWidth * float(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, input.texcoord).rgb;  
    }
    
    outputColor.rgb *= rcp(float(numSamples));
    
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    
    return output;
}