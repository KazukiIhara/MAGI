#include "Plane3D.hlsli"

PixelShaderOutput main(GeometryShaderOutput input)
{
    PixelShaderOutput output;
    output.color = input.color;
    return output;
}
