#include "GaussianBlur.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<GaussianBlurData> gdata : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    float2 uv = input.texcoord;
    float2 texOffset = float2(gdata.texelSize.x, 0.0f); // X•ûŒü‚Ì‚Ý

    float4 color = gTexture.Sample(gSampler, uv) * gdata.weights[0];

    [unroll]
    for (int i = 1; i <= 3; ++i)
    {
        float2 offset = texOffset * float(i);
        color += gTexture.Sample(gSampler, uv + offset) * gdata.weights[i];
        color += gTexture.Sample(gSampler, uv - offset) * gdata.weights[i];
    }

    output.color = color;
    return output;
}
