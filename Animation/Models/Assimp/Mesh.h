#include <string>
#include <memory>
#include <assimp/scene.h>
#include <unordered_map>
#include "Bone.h"
#include "../../../Render/Texture/Texture.h"
#include "../../Structures.h"

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