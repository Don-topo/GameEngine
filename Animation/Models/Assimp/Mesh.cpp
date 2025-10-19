#include "Mesh.h"

void Mesh::ProcessMesh(aiMesh* aiMesh, const aiScene* scene, std::string filePath, std::unordered_map<std::string, TextureData>& textures)
{
	// Set values
	name = aiMesh->mName.C_Str();
	numOfTriangles = aiMesh->mNumFaces;
	numOfVertex = aiMesh->mNumVertices;

	// AiMesh checks
	DEV_LOG(TE_INFO, "Mesh", "Mesh: %s has %i faces (%i vertices)", name.c_str(), numOfTriangles, numOfVertex);
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; i++)
	{
		if (aiMesh->HasVertexColors(i))
		{
			DEV_LOG(TE_INFO, "Mesh", "Mesh: %s has vertex color in color set: %i", name.c_str(), i);
		}
	}
	if (aiMesh->HasNormals())
	{
		DEV_LOG(TE_INFO, "Mesh", "Mesh: %s has normals", name.c_str());
	}
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; i++)
	{
		if (aiMesh->HasTextureCoords(i))
		{
			DEV_LOG(TE_INFO, "Mesh", "Mesh has texture coord in %i", i);
		}
	}

	// Load materials using texture
	aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
	if (material)
	{
		aiString materialName = material->GetName();
		DEV_LOG(TE_INFO, "Mesh", "The mesh has material, name: %s", materialName.C_Str());
		bool texturesFound = false;
		if (aiMesh->mMaterialIndex >= 0)
		{
			// scan only for diifuse and scalar textures for a start
			std::vector<aiTextureType> supportedTextureTypes = { aiTextureType_DIFFUSE, aiTextureType_SPECULAR };
			for (aiTextureType& textureType : supportedTextureTypes)
			{
				unsigned int numOfTextures = material->GetTextureCount(textureType);
				if (numOfTextures > 0)
				{
					DEV_LOG(TE_INFO, "Mesh", "material: %s has %i images of type %i", materialName.C_Str(), numOfTextures, textureType);
					for (unsigned int i = 0; i < numOfTextures; i++)
					{
						aiString textureName;
						material->GetTexture(textureType, i, &textureName);
						DEV_LOG(TE_INFO, "Mesh", "%s image %i has name %s", __FUNCTION__, i, textureName.C_Str());
						texturesFound = true;
						std::string texName = textureName.C_Str();

						mesh.textures.insert({ textureType, texName });
						/* skip already loaded textures */
						if (textures.count(texName) > 0)
						{
							DEV_LOG(TE_INFO, "Mesh", "%s: texture '%s' already loaded, skipping", __FUNCTION__, texName.c_str());
							continue;
						}
						// do not try to load internal textures
						if (!texName.empty() && texName.find("*") != 0)
						{
							TextureData newTexture{};
							// TODO need something to generate the full path
							std::string texturePath = texName;
							Texture newText;
							// TODO need Vulkan info to load textures
							//newText.LoadTexture()
							textures.insert({ texName, newText.GetTextureData()});
						}
					}
				}
			}
		}
		aiColor4D baseColor(0.f, 0.f, 0.f,1.f);
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) && AI_SUCCESS && texturesFound)
		{
			color = glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
			mesh.usesPBRColors = true;
		}
	}


}