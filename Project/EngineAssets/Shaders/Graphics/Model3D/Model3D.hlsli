// ─────────────────────────────────────
//  Model3D.hlsli   ―  共通シェーダー定義
// ─────────────────────────────────────

// ===== PS へ渡す頂点 =====
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};

// ===== 頂点／インスタンス／マテリアル =====
struct VertexData3D
{
    float4 position;
    float2 uv;
    float3 normal;
    float3 tangent;
};
struct ModelDataForGPU
{
    float4x4 worldMatrix;
};

struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _pad0;
    float4 baseColor;
    float4x4 uvMatrix;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _pad;
};

struct RootConstants
{
    uint baseInstanceIndex;
    uint _0, _1, _2;
};

// ===== Meshlet データ =====
struct Meshlet
{
    uint vertOffset; // gUniqueVert の先頭
    uint vertCount; // ≤ 64
    uint primOffset; // gPrimIB の先頭
    uint primCount; // ≤ 126
};

typedef uint PackedTriangle;

// ===== AS → MS Payload =====
struct ASPayload
{
    uint instanceID;
    uint meshletID;
};
