#include "Skeleton.h"

using namespace MAGIMath;

Skeleton::Skeleton(const Node& rootNode) {
	Initialize(rootNode);
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

	for (uint32_t i = 0; i < spheres_.size(); ++i) {
		Vector3 worldPos = ExtractionWorldPos(joints[i].skeletonSpaceMatrix);
		spheres_[i]->GetTranslate() = Vector3(worldPos);
		spheres_[i]->Update();
	}

}

void Skeleton::Draw() {
	for (auto& sphere : spheres_) {
		sphere->Draw();
	}
}

void Skeleton::Initialize(const Node& rootNode) {
	// ジョイントを作成
	root = CreateJoint(rootNode, {});
	// ジョイントマップを埋める
	for (const Joint& joint : joints) {
		jointMap.emplace(joint.name, joint.index);
	}

	Update();

	//
	// forDebug
	//

	for (uint32_t i = 0; i < jointMap.size(); i++) {
		std::unique_ptr<Primitive3D> newsphere = std::make_unique<Primitive3D>("sphere", Primitive3DType::Sphere);
		newsphere->GetScale() = Vector3(0.05f, 0.05f, 0.05f);
		spheres_.push_back(std::move(newsphere));
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