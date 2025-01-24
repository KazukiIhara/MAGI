#include "ModelDataContainer.h"

#include <iostream>
#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"

#include "AssetContainers/TextureDataContainer/TextureDataContainer.h"

using namespace MAGIMath;

ModelDataContainer::ModelDataContainer(TextureDataContainer* textureDataContainer) {
	Initialize(textureDataContainer);
	Logger::Log("ModelDataContainer Initialize\n");
}

ModelDataContainer::~ModelDataContainer() {
	Logger::Log("ModelDataContainer Finalize\n");
}

void ModelDataContainer::Initialize(TextureDataContainer* textureDataContainer) {
	SetTextureDataContainer(textureDataContainer);
	// コンテナをクリア
	modelDatas_.clear();
}

void ModelDataContainer::Load(const std::string& modelName, bool isNormalMap) {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(modelName)) {
		// 読み込み済みなら早期リターン
		return;
	}
	// モデルを読み込みコンテナに挿入
	modelDatas_.insert(std::make_pair(modelName, LoadModel(modelName, isNormalMap)));
}

ModelData ModelDataContainer::FindModelData(const std::string& modelName) const {
	// 読み込み済みモデルを検索
	if (modelDatas_.contains(modelName)) {
		// 読み込みモデルを戻り値としてreturn
		return modelDatas_.at(modelName);
	}
	// 見つからなかった場合止める

	assert(false && "Warning: Not found model");
	return ModelData{};
}

ModelData ModelDataContainer::LoadModel(const std::string& modelName, bool isNormalMap) {
	// 対応する拡張子のリスト
	std::vector<std::string> supportedExtensions = { ".obj", ".gltf" };

	// ディレクトリ内のファイルを検索
	std::string directoryPath = "App/Assets/Models";
	// モデルファイルが入っているディレクトリ
	std::string fileDirectoryPath = directoryPath + "/" + modelName;
	// filesystem用
	std::filesystem::path modelDirectoryPath(fileDirectoryPath);

	std::string modelFilePath;

	for (const auto& entry : std::filesystem::directory_iterator(modelDirectoryPath)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			// 対応する拡張子かチェック
			if (std::find(supportedExtensions.begin(), supportedExtensions.end(), ext) != supportedExtensions.end()) {
				if (entry.path().stem().string() == modelName) {
					modelFilePath = entry.path().string();
					break;
				}
			}
		}
	}

	// ファイルが見つからなかった場合はエラー
	if (modelFilePath.empty()) {
		std::cerr << "Error: Model file not found or unsupported format." << std::endl;
	}

	// 今回追加するモデルのデータ
	ModelData newModelData{};

	// 名前を設定
	newModelData.name = modelName;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFilePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate);
	assert(scene && scene->HasMeshes());

	std::vector<MaterialData> materials(scene->mNumMaterials);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		MaterialData materialData;

		// Diffuseテクスチャがある場合
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			materialData.textureFilePath = fileDirectoryPath + "/" + textureFilePath.C_Str();
			textureDataContainer_->Load(materialData.textureFilePath);

			// 法線マップがある場合の処理
			if (isNormalMap) {
				// 指定のパスを作成
				materialData.normalMapTextureFilePath = fileDirectoryPath + "/" + "Normal_" + textureFilePath.C_Str();
				// 法線マップテクスチャをロード
				textureDataContainer_->Load(materialData.normalMapTextureFilePath, true);
			}

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

		}

		newModelData.meshes.push_back(meshData);
	}

	return newModelData;
}

void ModelDataContainer::SetTextureDataContainer(TextureDataContainer* textureDataContainer) {
	assert(textureDataContainer);
	textureDataContainer_ = textureDataContainer;
}
