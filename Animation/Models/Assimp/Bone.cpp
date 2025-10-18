#include "Bone.h"

Bone::Bone(unsigned int id, std::string boneName, glm::mat4 offset)
{
	this->id = id;
	this->boneName = boneName;
	this->offset = offset;
	DEV_LOG(TE_INFO, "Bone", "Bone added!");
}