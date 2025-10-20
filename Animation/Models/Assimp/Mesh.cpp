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
	// TODO Move this to the render to be compatible with another graphics API
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

	// Prepare vertices
	for (unsigned int i = 0; i < numOfVertex; i++)
	{
		VkVertex vertex;
		// Cannot copy directly the complete value
		vertex.position.x = aiMesh->mVertices[i].x;
		vertex.position.y = aiMesh->mVertices[i].y;
		vertex.position.z = aiMesh->mVertices[i].z;

		// Prepare colors
		if (aiMesh->HasVertexColors(0))
		{
			// We have colors, only copy them
			vertex.color.r = aiMesh->mColors[0][i].r;
			vertex.color.g = aiMesh->mColors[0][i].g;
			vertex.color.b = aiMesh->mColors[0][i].b;
			vertex.color.a = aiMesh->mColors[0][i].a;
		}
		else
		{
			if (mesh.usesPBRColors)
			{
				vertex.color = color;
			}
			else
			{
				vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			}
		}

		// Prepare normals
		if (aiMesh->HasNormals())
		{
			vertex.normal.x = aiMesh->mNormals[i].x;
			vertex.normal.y = aiMesh->mNormals[i].y;
			vertex.normal.z = aiMesh->mNormals[i].z;
		}
		else
		{
			// Default values if not defined
			vertex.normal = glm::vec4(0.f);
		}

		// Prepare texture coordinates
		if (aiMesh->HasTextureCoords(0))
		{
			vertex.position.w = aiMesh->mTextureCoords[0][i].x;
			vertex.normal.w = aiMesh->mTextureCoords[0][i].x;
		}
		else
		{
			// Set default values
			vertex.position.w = 0.f;
			vertex.normal.w = 0.f;
		}

		mesh.vertices.emplace_back(vertex);
	}

	// Get triangles indices
	for (unsigned int i = 0; i < numOfTriangles; i++)
	{
		aiFace face = aiMesh->mFaces[i];
		// It's a triangle so only have three indices
		mesh.indices.push_back(face.mIndices[0]);
		mesh.indices.push_back(face.mIndices[1]);
		mesh.indices.push_back(face.mIndices[2]);
	}

	// Bones
	if (aiMesh->HasBones())
	{
		unsigned int numOfBones = aiMesh->mNumBones;
		for (unsigned int bone = 0; bone < numOfBones; bone++)
		{
			std::string boneName = aiMesh->mBones[bone]->mName.C_Str();
			unsigned int numWeights = aiMesh->mBones[bone]->mNumWeights;
			// Convert Ai to GLM
			glm::mat4 glmOffsetMatrix{
				aiMesh->mBones[bone]->mOffsetMatrix.a1, aiMesh->mBones[bone]->mOffsetMatrix.b1, aiMesh->mBones[bone]->mOffsetMatrix.c1, aiMesh->mBones[bone]->mOffsetMatrix.d1,
				aiMesh->mBones[bone]->mOffsetMatrix.a2, aiMesh->mBones[bone]->mOffsetMatrix.b2, aiMesh->mBones[bone]->mOffsetMatrix.c2, aiMesh->mBones[bone]->mOffsetMatrix.d2,
				aiMesh->mBones[bone]->mOffsetMatrix.a3, aiMesh->mBones[bone]->mOffsetMatrix.b3, aiMesh->mBones[bone]->mOffsetMatrix.c3, aiMesh->mBones[bone]->mOffsetMatrix.d3,
				aiMesh->mBones[bone]->mOffsetMatrix.a4, aiMesh->mBones[bone]->mOffsetMatrix.b4, aiMesh->mBones[bone]->mOffsetMatrix.c4, aiMesh->mBones[bone]->mOffsetMatrix.d4,
			};
			std::shared_ptr<Bone> newBone = std::make_unique<Bone>(bone, boneName, glmOffsetMatrix);
			bones.push_back(newBone);

			// Get bone weights
			for (unsigned int weight = 0; weight < numWeights; weight++)
			{
				unsigned int vertexId = aiMesh->mBones[bone]->mWeights[weight].mVertexId;
				float vertexWeight = aiMesh->mBones[bone]->mWeights[weight].mWeight;
				glm::vec4 boneIds = mesh.vertices.at(vertexId).boneNumber;
				glm::vec4 boneWeights = mesh.vertices.at(vertexId).boneWeight;

				for (unsigned int i = 0; i < 4; ++i) {
					if (boneWeights[i] == 0.0f) {
						boneIds[i] = bone;
						boneWeights[i] = vertexWeight;

						/* skip to next weight */
						break;
					}
				}

				mesh.vertices.at(vertexId).boneNumber = boneIds;
				mesh.vertices.at(vertexId).boneWeight = boneWeights;
			}

			// TODO Add animations here
		}
	}
}