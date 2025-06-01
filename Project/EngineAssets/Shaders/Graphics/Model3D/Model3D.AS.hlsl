#include "Model3D.hlsli"

ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<MeshInfo> gMeshInfo : register(b2);
ConstantBuffer<CameraFrustum> gFrustum : register(b3);

StructuredBuffer<ModelDataForGPU> gInstance : register(t0);
StructuredBuffer<CullData> gCullData : register(t5);

bool IsConeDegenerate(CullData c)
{
    return (c.NormalCone >> 24) == 0xff;
}

float4 UnpackCone(uint packed)
{
    float4 v;
    v.x = float((packed >> 0) & 0xFF);
    v.y = float((packed >> 8) & 0xFF);
    v.z = float((packed >> 16) & 0xFF);
    v.w = float((packed >> 24) & 0xFF);

    v = v / 255.0;
    v.xyz = v.xyz * 2.0 - 1.0;

    return v;
}

bool IsVisible(CullData c, float4x4 world, float3 viewPos)
{
    // Do a cull test of the bounding sphere against the view frustum planes.
    float4 center = mul(float4(c.BoundingSphere.xyz, 1), world);
    float radius = c.BoundingSphere.w;

    for (int i = 0; i < 6; ++i)
    {
        if (dot(center, gFrustum.planes[i]) < -radius)
        {
            return false;
        }
    }

    // Do normal cone culling
    if (IsConeDegenerate(c))
        return true; // Cone is degenerate - spread is wider than a hemisphere.

    // Unpack the normal cone from its 8-bit uint compression
    float4 normalCone = UnpackCone(c.NormalCone);

    // Transform axis to world space
    float3 axis = normalize(mul(float4(normalCone.xyz, 0), world)).xyz;

    // Offset the normal cone axis from the meshlet center-point - make sure to account for world scaling
    float3 apex = center.xyz - axis * c.ApexOffset;
    float3 view = normalize(viewPos - apex);

    // The normal cone w-component stores -cos(angle + 90 deg)
    // This is the min dot product along the inverted axis from which all the meshlet's triangles are backface
    if (dot(view, -axis) > normalCone.w)
    {
        return false;
    }

    // All tests passed - it will merit pixels
    return true;
}

[shader("amplification")]
[numthreads(AS_GROUP_SIZE, 1, 1)]
void main(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID)
{
    uint meshletID = gid.x * AS_GROUP_SIZE + dtid.x;
    uint instanceID = dtid.y;
    
    bool visible = false;
   
    Payload payload;
    payload.instanceID = instanceID;
    
    if (dtid.x < gMeshInfo.MeshletCount)
    {
        visible = IsVisible(gCullData[meshletID], gInstance[instanceID].world, gCamera.worldPosition);
    }
    
    if (visible)
    {
        uint index = WavePrefixCountBits(visible);
        payload.meshletIndices[index] = meshletID;
    }
    
    uint visibleCount = WaveActiveCountBits(visible);

    DispatchMesh(visibleCount, 1, 1, payload);
}
