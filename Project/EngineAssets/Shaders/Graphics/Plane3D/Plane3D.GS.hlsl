#include "Plane3D.hlsli"

StructuredBuffer<PlaneData3D> gInstanceData : register(t0);
ConstantBuffer<Camera> gCamera : register(b0);

[maxvertexcount(4)]
void main(
    point VertexShaderOutput input[1],
    inout TriangleStream<GeometryShaderOutput> stream
)
{
    uint instanceID = input[0].instanceID;
    PlaneData3D plane = gInstanceData[instanceID];

    // UV順（左上, 右上, 左下, 右下）
    float2 uvs[4] =
    {
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f)
    };

    // 三角形ストリップ順序でループ（0,1,2,3）
    [unroll]
    for (uint i = 0; i < 4; ++i)
    {
        float4 localPos = plane.vertices[i];
        float4 worldPos = mul(localPos, plane.worldMatrix);

        GeometryShaderOutput output;
        output.position = mul(worldPos, gCamera.viewProjection);
        output.texcoord = uvs[i];
        output.normal = normalize(mul(float4(0.0f, 1.0f, 0.0f, 0.0f), plane.worldInverseTranspose).xyz);
        output.tangent = normalize(mul(float4(1.0f, 0.0f, 0.0f, 0.0f), plane.worldInverseTranspose).xyz);
        output.instanceID = instanceID;

        stream.Append(output);
    }
}
