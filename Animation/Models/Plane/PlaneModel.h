#pragma once

#include <glm/glm.hpp>
#include "../../../Logger/Logger.h"
#include "../../../Render/VertexBuffer/VertexBuffer.h"

class PlaneModel
{
public:
	PlaneModel(const float height = 1.f, const float width = 1.f, const glm::vec3 color = glm::vec3(1.f));
	LineMesh& GetMeshVertexs() { return planeMesh; }
private:
	void Initialization();
	float height = 1.f;
	float width = 1.f;
	glm::vec3 color = glm::vec3(1.f);
	LineMesh planeMesh = {};
};