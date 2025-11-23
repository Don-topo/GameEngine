#include "Model.h"

Model::Model()
{
}

void Model::LoadModel(std::string fileName, unsigned int extraImportFlags)
{
	Assimp::Importer assimpImporter;
	const aiScene* scene = assimpImporter.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ValidateDataStructure | aiProcess_FlipUVs | extraImportFlags);

	if (!scene || !scene->mRootNode)
	{
		DEV_LOG(TE_ERROR, "Model", "Error loading model with assimp!");
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		numOfVertex += scene->mMeshes[i]->mNumVertices;
		numOfFaces += scene->mMeshes[i]->mNumFaces;
	}

	aiNode* rootNode = scene->mRootNode;
	
	// Check if the model has textures
	if (scene->HasTextures())
	{
		// Load textures
		for (unsigned int i = 0; i < scene->mNumTextures; i++)
		{
			// Get texture info
			std::string name = scene->mTextures[i]->mFilename.C_Str();
			int height = scene->mTextures[i]->mHeight;
			int width = scene->mTextures[i]->mWidth;
			aiTexel* textureData = scene->mTextures[i]->pcData;

			// Load the texture itself
			Texture texture;
			//texture.LoadTexture();
		}
		
	
	}
}

void Model::ProcessNode(std::shared_ptr<Node> node, aiNode* aNode, const aiScene* scene, std::string assetDirectory)
{
	std::string name = aNode->mName.C_Str();
	unsigned int numMeshes = aNode->mNumMeshes;

	if (numMeshes > 0)
	{
		for (unsigned int i = 0; i < numMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[aNode->mMeshes[i]];
			//Mesh mesh;
			//mesh.ProcessMesh(aiMesh, scene, assetDirectory,)
		}
	}
}
