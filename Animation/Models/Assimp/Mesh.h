#include <string>
#include <memory>
#include <assimp/scene.h>
#include <unordered_map>
#include "Bone.h"
#include "../../../Render/Texture/Texture.h"

struct VkMorphVertex {
	glm::vec4 position = glm::vec4(0.0f);
	glm::vec4 normal = glm::vec4(0.0f);
};

struct VkMorphMesh {
	std::vector<VkMorphVertex> morphVertices{};
};

struct VkVertex {
	glm::vec4 position = glm::vec4(0.0f); // last float is uv.x
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec4 normal = glm::vec4(0.0f); // last float is uv.y
	glm::uvec4 boneNumber = glm::uvec4(0);
	glm::vec4 boneWeight = glm::vec4(0.0f);
};

struct VkMesh {
	std::vector<VkVertex> vertices{};
	std::vector<uint32_t> indices{};
	std::unordered_map<aiTextureType, std::string> textures{};
	bool usesPBRColors = false;
	/* store optional morph meshes directly in renderer mesh */
	std::vector<VkMorphMesh> morphMeshes{};
};

class Mesh
{
public:
	void ProcessMesh(aiMesh* aiMesh, const aiScene* scene, std::string filePath, std::unordered_map<std::string, TextureData> &textures);
	std::string GetName() { return name; }
	unsigned int GetNumOfTriangles() { return numOfTriangles; }
	unsigned int GetNumOfVertex() { return numOfVertex; }
	std::vector<std::shared_ptr<Bone>> GetBones() { return bones; }
	VkMesh GetMesh() { return mesh; }
	std::vector<uint32_t> GetIndices() { mesh.indices; }

private:
	std::string name;
	unsigned int numOfTriangles;
	unsigned int numOfVertex;

	glm::vec4 color;
	std::vector<std::shared_ptr<Bone>> bones;
	VkMesh mesh;
};