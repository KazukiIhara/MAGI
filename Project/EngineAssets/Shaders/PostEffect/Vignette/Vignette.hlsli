struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct VignetteData
{
    float scale;
    float falloff;
    float _padding0;
    float _padding1;
};