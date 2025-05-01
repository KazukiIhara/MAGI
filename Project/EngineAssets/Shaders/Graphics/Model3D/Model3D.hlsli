// メッシュシェーダーからピクセルシェーダーに渡すデータ
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// 頂点データ
struct VertexData3D
{
    float4 position;
    float2 uv;
    float3 normal;
    float3 tangent;
};

// インスタンシングデータ
struct ModelDataForGPU
{
    float4x4 worldMatrix;
};

// マテリアルデータ
struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float4x4 uvMatrix;
};

// カメラデータ
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

// AS用Payload
struct ASPayload
{
    uint instanceID;
};
