#include "Sphere3D.hlsli"

StructuredBuffer<SphereData3D> gInstanceData : register(t0);
ConstantBuffer<Camera> gCamera : register(b0);

[maxvertexcount(96)] // ※必要に応じて調整
void main(
    point VertexShaderOutput input[1],
    inout TriangleStream<GeometryShaderOutput> stream
)
{
    uint instanceID = input[0].instanceID;
    SphereData3D sphere = gInstanceData[instanceID];

    float radius = sphere.radius;
    uint lon = max(sphere.longitudeSegments, 3);
    uint lat = max(sphere.latitudeSegments, 2);

    float latStep = 3.141592f / float(lat); // π / lat
    float lonStep = 2.0f * 3.141592f / float(lon); // 2π / lon

    for (uint y = 0; y < lat; ++y)
    {
        float theta0 = -3.141592f / 2.0f + latStep * y;
        float theta1 = theta0 + latStep;

        float sinTheta0 = sin(theta0), cosTheta0 = cos(theta0);
        float sinTheta1 = sin(theta1), cosTheta1 = cos(theta1);

        for (uint x = 0; x < lon; ++x)
        {
            float phi0 = lonStep * x;
            float phi1 = phi0 + lonStep;

            float sinPhi0 = sin(phi0), cosPhi0 = cos(phi0);
            float sinPhi1 = sin(phi1), cosPhi1 = cos(phi1);

            // 頂点のローカル位置
            float3 p00 = float3(cosTheta0 * cosPhi0, sinTheta0, cosTheta0 * sinPhi0) * radius;
            float3 p01 = float3(cosTheta0 * cosPhi1, sinTheta0, cosTheta0 * sinPhi1) * radius;
            float3 p10 = float3(cosTheta1 * cosPhi0, sinTheta1, cosTheta1 * sinPhi0) * radius;
            float3 p11 = float3(cosTheta1 * cosPhi1, sinTheta1, cosTheta1 * sinPhi1) * radius;

            // ワールド変換
            float4 wp00 = mul(float4(p00, 1.0f), sphere.worldMatrix);
            float4 wp01 = mul(float4(p01, 1.0f), sphere.worldMatrix);
            float4 wp10 = mul(float4(p10, 1.0f), sphere.worldMatrix);
            float4 wp11 = mul(float4(p11, 1.0f), sphere.worldMatrix);

            // 法線（反転して外向きに）
            float3 normal00 = -normalize(mul(float4(p00, 0.0f), sphere.worldInverseTranspose).xyz);
            float3 normal01 = -normalize(mul(float4(p01, 0.0f), sphere.worldInverseTranspose).xyz);
            float3 normal10 = -normalize(mul(float4(p10, 0.0f), sphere.worldInverseTranspose).xyz);
            float3 normal11 = -normalize(mul(float4(p11, 0.0f), sphere.worldInverseTranspose).xyz);

            // タンジェント（経度方向に向ける）
            float3 tangent = normalize(mul(float4(-sin(phi0), 0.0f, cos(phi0), 0.0f), sphere.worldInverseTranspose).xyz);

            // UV（OpenGLスタイルで1.0 - lat）
            float2 uv00 = float2((float) x / lon, 1.0f - (float) y / lat);
            float2 uv01 = float2((float) (x + 1) / lon, 1.0f - (float) y / lat);
            float2 uv10 = float2((float) x / lon, 1.0f - (float) (y + 1) / lat);
            float2 uv11 = float2((float) (x + 1) / lon, 1.0f - (float) (y + 1) / lat);

            // 三角形1
            GeometryShaderOutput o;
            o.instanceID = instanceID;

            o.normal = normal00;
            o.texcoord = uv00;
            o.position = mul(wp00, gCamera.viewProjection);
            stream.Append(o);

            o.normal = normal10;
            o.texcoord = uv10;
            o.position = mul(wp10, gCamera.viewProjection);
            stream.Append(o);

            o.normal = normal11;
            o.texcoord = uv11;
            o.position = mul(wp11, gCamera.viewProjection);
            stream.Append(o);

            stream.RestartStrip();

            // 三角形2
            o.normal = normal00;
            o.texcoord = uv00;
            o.position = mul(wp00, gCamera.viewProjection);
            stream.Append(o);

            o.normal = normal11;
            o.texcoord = uv11;
            o.position = mul(wp11, gCamera.viewProjection);
            stream.Append(o);

            o.normal = normal01;
            o.texcoord = uv01;
            o.position = mul(wp01, gCamera.viewProjection);
            stream.Append(o);

            stream.RestartStrip();
        }
    }
}
