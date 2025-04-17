
// Meshシェーダーからピクセルシェーダーに渡す構造体
struct MeshOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

// インスタンシングデータ
struct TriangleData3D
{
    float4x4 worldMatrix;
    float4 color;
};

// カメラ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};