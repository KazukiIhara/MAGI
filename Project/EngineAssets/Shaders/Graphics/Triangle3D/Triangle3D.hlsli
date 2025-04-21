
// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// インスタンシングデータ
struct TriangleData3D
{
    float4x4 worldMatrix;
    float4 vertices[3];
};

// マテリアルデータ
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float4x4 uvMatrix;
};

// カメラ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};

// ルート定数
struct RootConstants
{
    uint baseInstanceIndex;
    uint _pad0, _pad1, _pad2;
};

// ASで使う構造体
struct ASPayload
{
    uint instanceID;
    float markerValue;
};