// ────────── Model3D.hlsli (Mesh-only) ──────────
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instID : TEXCOORD1;
};

// 頂点・インスタンス・マテリアル
struct VertexData3D
{
    float4 pos;
    float2 uv;
    float3 nrm;
    float3 tan;
};
struct ModelDataForGPU
{
    float4x4 world;
};
struct PrimitiveMaterialData3D
{
    uint texIdx;
    float3 _pad;
    float4 baseColor;
    float4x4 uvMatrix;
};
struct Camera
{
    float4x4 vp;
    float3 eye;
    float _;
};

// Meshlet header（DXMesh v168）
struct Meshlet
{
    uint vertOffset, vertCount;
    uint primOffset, primCount;
};

// 10-bit×3 packed triangle (= uint32)
typedef uint PackedTriangle;
