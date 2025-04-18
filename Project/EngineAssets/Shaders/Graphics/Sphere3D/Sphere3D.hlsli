struct MeshOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    uint instanceIndex : TEXCOORD1;
};

struct SphereData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    float radius;
    uint longitudeSegments;
    uint latitudeSegments;
    float _padding;
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
