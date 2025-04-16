#include "Triangle3D.hlsli"

[numthreads(1, 1, 1)]
[outputtopology("triangle")]
void main(
    out indices uint3 tris[1],
    out vertices MeshOutput verts[3]
)
{
    // 出力する頂点数と三角形数を明示
    SetMeshOutputCounts(3, 1);

    // 頂点データ
    verts[0].position = float4(-0.5f, -0.5f, 0.0f, 1.0f);
    verts[0].color = float4(1, 0, 0, 1); // 赤

    verts[1].position = float4(0.0f, 0.5f, 0.0f, 1.0f);
    verts[1].color = float4(0, 1, 0, 1); // 緑

    verts[2].position = float4(0.5f, -0.5f, 0.0f, 1.0f);
    verts[2].color = float4(0, 0, 1, 1); // 青

    // 三角形のインデックス
    tris[0] = uint3(0, 1, 2);
}
