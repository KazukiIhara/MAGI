#include "Object3D.hlsli"

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, mul(gTransformationMatrix.World, gCamera.viewProjection));
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.WorldInverseTranspose));
    output.tangent = input.tangent;
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    return output;
}