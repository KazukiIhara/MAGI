
static const float NormalBiasScale = 0.0005f;

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// カメラデータ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _pad;
};

struct DirectionalLightData
{
    float3 direction;
    float intencity;
    float3 color;
    float padding;
};

struct LightCameraData
{
    float4x4 viewProjection;
};