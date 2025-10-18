#pragma once

#include "../../../Logger/Logger.h"
#include <string>
#include "glm/glm.hpp"

class Bone
{
public:
	Bone(unsigned int id, std::string boneName, glm::mat4 offset);
	unsigned int GetBoneId() { return id; }
	std::string GetBoneName() { return boneName; }
	glm::mat4 GetOffset() { return offset; }

private:
	unsigned int id;
	std::string boneName;
	glm::mat4 offset;
};