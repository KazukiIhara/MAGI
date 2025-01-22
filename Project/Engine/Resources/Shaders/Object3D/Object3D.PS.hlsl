#include "Object3D.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);
ConstantBuffer<ModelMaterial> gModelMaterial : register(b2);
ConstantBuffer<LightCount> gLightCount : register(b3);
Texture2D<float4> gTexture : register(t0);
StructuredBuffer<PunctualLight> gLights : register(t1);
SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = float4(0, 0, 0, 1);

    // �O����
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    // Diffuse,Specular��~�ς��邽�߂̕ϐ�
    float3 totalDiffuse = 0.0f;
    float3 totalSpecular = 0.0f;
    
    // UV�����擾
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), mul(gMaterial.uvTransform, gModelMaterial.uvTransform));
    // �e�N�X�`���T���v�����O
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // discard����
    if (textureColor.a <= 0.5f)
    {
        discard;
    }
    
    // ���̃J���[��ۑ�
    float3 baseColor = gModelMaterial.color.rgb * gMaterial.color.rgb * textureColor.rgb;
    float alpha = gModelMaterial.color.a * gMaterial.color.a * textureColor.a;
     
    if (gMaterial.enableLighting != 0)
    {
        // �V�[�����̂��ׂẴ��C�g�����[�v
        [loop]
        for (uint i = 0; i < gLightCount.num; i++)
        {
            PunctualLight light = gLights[i];

            switch (light.type)
            {
                case 0: // Directional
            {
                        float3 L = -normalize(light.direction);
                        float NdotL = saturate(dot(normal, L));
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));

                        // Diffuse
                        float3 diffuse = light.color * light.intensity * NdotL;
                        totalDiffuse += diffuse;

                        // Specular
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow;
                        totalSpecular += specular;
                    }
                    break;

                case 1: // Point
            {
                        float3 lightDir = light.position - input.worldPosition;
                        float distance = length(lightDir);
                        float3 L = normalize(lightDir);

                        // ��������
                        float atten = pow(saturate(-distance / light.radius + 1.0f), light.decay);

                        // Diffuse
                        float NdotL = saturate(dot(normal, L));
                        float3 diffuse = light.color * light.intensity * NdotL * atten;
                        totalDiffuse += diffuse;

                        // Specular
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow * atten;
                        totalSpecular += specular;
                    }
                    break;

                case 2: // Spot
            {
                        float3 lightDir = light.position - input.worldPosition;
                        float distance = length(lightDir);
                        float3 L = normalize(lightDir);

                        // ��������
                        float atten = pow(saturate(-distance / light.radius + 1.0f), light.decay);

                        // �X�|�b�g���C�g�̊p�x�ɂ�錸��
                        float cosAngle = dot(normalize(lightDir), normalize(light.direction));
                        float falloff = saturate((cosAngle - light.cosAngle) /
                                          (light.cosFalloffStart - light.cosAngle));

                        // Diffuse
                        float NdotL = saturate(dot(normal, L));
                        float3 diffuse = light.color * light.intensity * NdotL * atten * falloff;
                        totalDiffuse += diffuse;

                        // Specular
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow * atten * falloff;
                        totalSpecular += specular;
                    }
                    break;
            
            }
        }
        
        // �ݐς������C�g�̒l�����Z
        output.color.rgb = baseColor * totalDiffuse + totalSpecular;
        output.color.a = alpha;
    }
    else
    {
        // ���C�g��K�p���Ȃ��ꍇ�͂��̂܂�
        output.color.rgb = baseColor;
        output.color.a = alpha;
    }
    
    
    // �A���t�@�J�b�g
    if (output.color.a <= 0.0f)
    {
        discard;
    }
       
    return output;
}
