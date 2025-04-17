#include "Triangle3D.hlsli"

float4 main(MeshOutput input) : SV_Target
{
    return input.color;
}