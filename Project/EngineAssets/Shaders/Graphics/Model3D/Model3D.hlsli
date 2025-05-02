// ────────── Model3D.hlsli ──────────

#define THREADS_PER_WAVE 32
#define AS_GROUP_SIZE THREADS_PER_WAVE

// 頂点出力
struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instID : TEXCOORD1;
    uint meshletIndex : COLOR0;
};

// 頂点データ
struct VertexData3D
{
    float4 position;
    float2 uv;
    float3 normal;
    float3 tangent;
};

struct VertexOutPut
{
    float4 position;
    float2 uv;
    uint meshletIndex;
};

// インスタンスデータ
struct ModelDataForGPU
{
    float4x4 world;
};

// マテリアルデータ
struct MaterialData3D
{
    uint texIdx;
    float3 _pad;
    float4 baseColor;
    float4x4 uvMatrix;
};

// カメラデータ
struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _pad;
};

// ────────── Meshlet関連構造体 ──────────

struct MeshInfo
{
    uint IndexSize;
    uint MeshletCount;

    uint LastMeshletVertCount;
    uint LastMeshletPrimCount;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};

struct Payload          // AS → MS で渡す最小ペイロード
{
    uint meshletIndices[AS_GROUP_SIZE];
    uint instanceID;
    uint _pad[2];
};