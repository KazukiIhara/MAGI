#include "Skeleton.h"

using namespace MAGIMath;

Skeleton::Skeleton(const Node& rootNode, const std::unordered_map<std::string, Matrix4x4>& ibpMap) {
	Initialize(rootNode, ibpMap);
}

Skeleton::~Skeleton() {}

void Skeleton::Update() {
	for (Joint& joint : joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * joints[*joint.parent].skeletonSpaceMatrix;
		} else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Skeleton::Initialize(const Node& rootNode, const std::unordered_map<std::string, Matrix4x4>& ibpMap) {
	// ジョイントを作成
	root = CreateJoint(rootNode, {});
	// ジョイントマップを埋める
	for (const Joint& joint : joints) {
		jointMap.emplace(joint.name, joint.index);
	}
	// ジョイントマップに含まれるジョイント名を調べて、同名のジョイントがあれば inverseBindPose を設定する
	for (auto& [jointName, ibp] : ibpMap) {
		auto it = jointMap.find(jointName);
		if (it != jointMap.end()) {
			int jointIndex = it->second;
			joints[jointIndex].inverseBindPoseMatrix = ibp;
		}
	}
}

int32_t Skeleton::CreateJoint(const Node& node, const std::optional<int32_t>& parent) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentityMatrix4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index);
		joints[joint.index].children.push_back(childIndex);
	}
	return joint.index;
}