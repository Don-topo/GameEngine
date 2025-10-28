#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../Logger/Logger.h"
#include "VkBootstrap.h"
#include "../../Animation/Models/Assimp/Node.h"
#include "../../Animation/Models/Assimp/Bone.h"
#include "../../AABB/AABB.h"

class Model
{
public:
	Model();
	//Draw();
	unsigned int GetNumOfTriangles();
	std::string GetModelFileName();
	std::string GetModelFileNamePath();
	std::vector<std::shared_ptr<Node>>& GetNodes();
	std::unordered_map<std::string, std::shared_ptr<Node>>& GetMapNodes();
	std::vector<std::shared_ptr<Bone>>& GetBones();
	std::vector<std::string> GetBonesNames();


private:

};