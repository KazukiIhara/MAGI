struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct GaussianBlurData
{
    float2 texelSize;
    float weights[7];
    float3 _padding;
};