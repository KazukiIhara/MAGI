
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 worldPosition : POSITION0;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct PlaneData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float3 leftTop;
    float3 rightTop;
    float3 leftBottom;
    float3 rightBottom;
    float4 color;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};