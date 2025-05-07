struct CylinderData3D
{
    float4x4 worldMatrix;
    float4x4 worldInverseTranspose;
    uint cylinderDivide;
    float topRadius;
    float bottomRadius;
    float height;
};

// ƒ^ƒCƒ‹‚ÌÅ‘å”
static const uint kMaxTileCount = 256 * 256;