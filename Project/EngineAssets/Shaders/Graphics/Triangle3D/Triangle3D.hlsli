
// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    uint instanceIndex : TEXCOORD1;
};

// インスタンシングデータ
struct TriangleData3D
{
    float4x4 worldMatrix;
};

// マテリアルデータ
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float2 uvTranslate;
    float2 uvScale;
    float uvRotate;
    float3 _padding1;
};

// カメラ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};