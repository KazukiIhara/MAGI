
// 頂点シェーダ入力
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

// 頂点シェーダからの出力 → ジオメトリシェーダへ
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 worldPosition : POSITION0;
    uint instanceID : SV_InstanceID;
};

// ジオメトリシェーダからの出力 → ピクセルシェーダへ
struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    uint instanceID : TEXCOORD1;
};

// ピクセルシェーダ出力
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// インスタンシングデータ
struct SphereData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float radius;
    uint longitudeSegments;
    uint latitudeSegments;
    float _padding0;
};

// マテリアルデータ（StructuredBuffer用）
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;

    float4 baseColor;

    float2 uvTransform;
    float2 uvScale;
    float uvRotation;
    float3 _padding1;
};

// カメラデータ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};