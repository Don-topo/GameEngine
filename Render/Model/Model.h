#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../Logger/Logger.h"
#include "VkBootstrap.h"
#include "../../Animation/Models/Assimp/Node.h"
#include "../../Animation/Models/Assimp/Bone.h"
#include "../../Animation/Models/Assimp/Mesh.h"
#include "../../AABB/AABB.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "../Texture/Texture.h"


class Model
{
public:
	Model();
	void LoadModel(std::string fileName, unsigned int extraImportFlags = 0);
	void Draw();
	void Cleanup();
	//void DrawInstanced();
	//unsigned int GetNumOfTriangles();
	/*std::string GetModelFileName();
	std::string GetModelFileNamePath();
	std::vector<std::shared_ptr<Node>>& GetNodes();
	std::unordered_map<std::string, std::shared_ptr<Node>>& GetMapNodes();
	std::vector<std::shared_ptr<Bone>>& GetBones();
	std::vector<std::string> GetBonesNames();*/
	

private:
	void ProcessNode(std::shared_ptr<Node> node, aiNode* aNode, const aiScene* scene, std::string assetDirectory);
	unsigned int numOfVertex = 0;
	unsigned int numOfFaces = 0;
	unsigned int numOfTriangles = 0;
	std::shared_ptr<std::shared_ptr<Node>> nodes;
	std::vector<std::shared_ptr<Bone>> bones;
	std::vector<std::string> boneNames;

};