#pragma once

#include <glm/glm.hpp>
#include "../../../Logger/Logger.h"
#include "../../../Render/VertexBuffer/VertexBuffer.h"

class SphereModel
{
public:
	SphereModel(const float radius = 1.0f, const float verticalDiv = 5, const float horizontalDiv = 8, const glm::vec3 color = glm::vec3(1.0f)) :
		sphereRadius(radius), vertDiv(verticalDiv), hortDiv(horizontalDiv), color(color) {
		Initialization();
	};
	LineMesh GetSphereVertexs() { return sphereMesh; }

private:
	void Initialization();
	float sphereRadius = 1.f;
	unsigned int vertDiv = 10;
	unsigned int hortDiv = 20;
	glm::vec3 color = glm::vec3(1.f);
	LineMesh sphereMesh = {};
};