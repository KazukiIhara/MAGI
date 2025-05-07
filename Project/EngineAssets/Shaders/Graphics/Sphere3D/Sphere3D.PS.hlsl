#include "Sphere3D.hlsli"
#include "../Primitive3D/Primitive3D.hlsli"

Texture2D gTextures[] : register(t1000);
SamplerState gSampler : register(s0);
StructuredBuffer<PrimitiveMaterialData3D> gMaterialData : register(t1);

// ‚R‚Â‚ÌG-Buffer‚Éo—Í‚·‚é‚½‚ß‚Ì\‘¢‘Ì
struct GBufferOutput
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
};

GBufferOutput main(MeshOutput input)
{
    GBufferOutput output;
    
    uint instanceID = input.instanceIndex;
    PrimitiveMaterialData3D mat = gMaterialData[instanceID];

    float2 uv = mul(float4(input.uv, 0.0f, 1.0f), mat.uvMatrix).xy;
    float4 texColor = gTextures[mat.textureIndex].Sample(gSampler, uv);
    output.albedo = texColor * mat.baseColor;
    
    float3 normal = normalize(input.normal); // ”O‚Ì‚½‚ß³‹K‰»
    output.normal = float4(normal * 0.5f + 0.5f, 1.0f); // [-1,1] ¨ [0,1]ƒ}ƒbƒsƒ“ƒO‚µ‚ÄŠi”[
    
    output.position = input.worldPosition;
    
    return output;
}