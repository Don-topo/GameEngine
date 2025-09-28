#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../../../Logger/Logger.h"

struct SkyboxVertex {
	glm::vec4 position = glm::vec4(0.0f);
};

struct SkyboxMesh {
	std::vector<SkyboxVertex> vertices{};
};

class SkyboxModel
{
public:
	void Initialization();
	SkyboxMesh GetSkyboxVertex() { return skyboxVertex; }
private:
	SkyboxMesh skyboxVertex = {};
};
