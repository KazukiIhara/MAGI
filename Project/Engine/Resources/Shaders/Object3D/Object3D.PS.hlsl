#include "Object3D.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<ModelMaterial> gModelMaterial : register(b3);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

