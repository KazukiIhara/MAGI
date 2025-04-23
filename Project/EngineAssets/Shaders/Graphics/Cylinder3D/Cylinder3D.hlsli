struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    uint instanceIndex : TEXCOORD1;
};


struct CylinderData3D
{
    float4x4 worldMatrix;
    uint cylinderDivide;
    float topRadius;
    float bottomRadius;
    float height;
    
    float3 _padding;
};

struct PrimitiveMaterialData3D
{
    uint textureIndex;
    float3 _padding0;
    float4 baseColor;
    float4x4 uvMatrix;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
    float _padding;
};

struct TempVertex
{
    float4 position;
    float2 uv;
    float4 color;
    uint instanceIndex;
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
};

// タイルの最大数
static const uint kMaxTileCount = 256 * 256;