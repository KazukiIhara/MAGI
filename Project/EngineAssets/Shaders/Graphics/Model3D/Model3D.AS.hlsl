//#include "Model3D.hlsli"

//ConstantBuffer<RootConstants> gRootConst : register(b2);
//StructuredBuffer<Meshlet> gMeshlets : register(t3);

//[numthreads(1, 1, 1)]
//void main(uint3 gid : SV_GroupID)
//{
//    ASPayload p;
//    p.instanceID = gRootConst.baseInstanceIndex + gid.y;
//    p.meshletID = gid.x; // 0ÅcmeshletCount_-1

//    DispatchMesh(1, 1, 1, p);
//}
