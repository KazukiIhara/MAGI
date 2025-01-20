#include "ModelDataContainer.h"

#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"


ModelDataContainer::ModelDataContainer() {

}

ModelDataContainer::~ModelDataContainer() {
}

void ModelDataContainer::Initialize() {
}

void ModelDataContainer::Load(const std::string& fileName) {
}

ModelData ModelDataContainer::FindModelData(const std::string& fileName) const {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(fileName)) {
		// 読み込みモデルを戻り値としてreturn
		return modelDatas_.at(fileName);
	} else {
		assert(false);
	}
}

ModelData ModelDataContainer::LoadModel(const std::string& filePath) {
	// 対応する拡張子のリスト
	std::vector<std::string> supportedExtensions = { ".obj", ".gltf" };

	// ディレクトリ内のファイルを検索
	// モデルファイルが入っているディレクトリ
	std::string fileDirectoryPath = directoryPath + "/" + filename;
	// filesystem用
	std::filesystem::path modelDirectoryPath(fileDirectoryPath);

	std::string modelFilePath;

	for (const auto& entry : std::filesystem::directory_iterator(modelDirectoryPath)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			// 対応する拡張子かチェック
			if (std::find(supportedExtensions.begin(), supportedExtensions.end(), ext) != supportedExtensions.end()) {
				if (entry.path().stem().string() == filename) {
					modelFilePath = entry.path().string();
					break;
				}
			}
		}
	}

	// ファイルが見つからなかった場合はエラー
	if (modelFilePath.empty()) {
		std::cerr << "Error: Model file not found or unsupported format." << std::endl;
		return;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFilePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate);
	assert(scene && scene->HasMeshes());

	// ノード読み込み
	modelData_.rootNode = ReadNode(scene->mRootNode);

	std::vector<MaterialData> materials(scene->mNumMaterials);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		MaterialData materialData;

		// Diffuseテクスチャがある場合の処理
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			materialData.textureFilePath = fileDirectoryPath + "/" + textureFilePath.C_Str();
			SUGER::LoadTexture(materialData.textureFilePath);
			materialData.haveUV_ = true;

			// UVスケール情報の取得
			aiUVTransform uvTransform;
			if (material->Get(AI_MATKEY_UVTRANSFORM(aiTextureType_DIFFUSE, 0), uvTransform) == AI_SUCCESS) {
				Vector2 scale = { uvTransform.mScaling.x, uvTransform.mScaling.y };
				float rotateZ = uvTransform.mRotation; // Z軸回転
				Vector2 translate = { uvTransform.mTranslation.x, uvTransform.mTranslation.y };

				// UVマトリックスを構築
				materialData.uvMatrix = MakeUVMatrix(scale, rotateZ, translate);
			} else {
				// UV変換が見つからない場合は単位行列を設定
				materialData.uvMatrix = MakeIdentityMatrix4x4();
			}
		} else {
			materialData.haveUV_ = false;
		}

		aiColor4D baseColor;
		// PBRマテリアルの BaseColorFactor の取得
		if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
			// BaseColorが存在する場合
			materialData.color = { baseColor.r, baseColor.g, baseColor.b, baseColor.a };
		} else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS) {
			// Fallback: DiffuseColorの取得
			materialData.color = { baseColor.r, baseColor.g, baseColor.b, baseColor.a };
		} else {
			// デフォルト色（白）
			materialData.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		materials[materialIndex] = materialData;
	}

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());

		MeshData meshData;
		meshData.material = materials[mesh->mMaterialIndex];


		if (mesh->HasTextureCoords(0)) { // UVあり

			// 最初に頂点数分のメモリを確保
			meshData.vertices.resize(mesh->mNumVertices);

			// 頂点解析
			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				meshData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
				meshData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
				meshData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
			}
			// index解析
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					meshData.indices.push_back(vertexIndex);
				}
			}

			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
				aiBone* bone = mesh->mBones[boneIndex];
				std::string jointName = bone->mName.C_Str();
				JointWeightData& jointWeightData = modelData_.skinClusterData[jointName];

				aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
				aiVector3D scale, translate;
				aiQuaternion rotate;
				bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
				Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
					{ scale.x,scale.y,scale.z },
					{ rotate.x,-rotate.y,-rotate.z,rotate.w },
					{ -translate.x,translate.y,translate.z }
				);
				jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

				for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
					jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
				}

			}

		} else { // UVなし
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					aiVector3D& position = mesh->mVertices[vertexIndex];
					aiVector3D& normal = mesh->mNormals[vertexIndex];
					VertexData3DUnUV vertex;
					vertex.position = { position.x, position.y, position.z, 1.0f };
					vertex.normal = { normal.x, normal.y, normal.z };
					vertex.position.x *= -1.0f;
					vertex.normal.x *= -1.0f;
					meshData.verticesUnUV.push_back(vertex);
				}
			}
		}

		modelData_.meshes.push_back(meshData);
	}
}
