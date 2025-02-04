struct VertexShaderInput
{
    uint instanceId : SV_InstanceID; // �C���X�^���V���O�p
    uint vertexId : SV_VertexID; // ���_ID
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION; // �X�N���[�����W
    float4 color : COLOR0; // ���C���̐F
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0; // �o�͐F
};

// 3D���C���f�[�^
struct LineData3D
{
    float3 start;
    float3 end;
    float4 color;
};

struct Camera
{
    float4x4 viewProjection;
    float3 worldPosition;
};