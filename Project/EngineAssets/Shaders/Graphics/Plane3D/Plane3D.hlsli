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
    float3 tangent : TANGENT;
    uint instanceID : TEXCOORD1;
};

// ピクセルシェーダの出力
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// インスタンシングデータ
struct PlaneData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float4 vertices[4]; // ローカル空間の4頂点
};

// マテリアルデータ
struct PlaneMaterialData3D
{
    uint textureIndex; // gTextures[] 内のインデックス
    float4 baseColor;
    float2 uvTranslate;
    float2 uvScale;
    float uvRotate;
};

// カメラ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};