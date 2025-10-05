#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "../Render/VertexBuffer/VertexBuffer.h"

class AABB
{
public:
	AABB();

	void Create(glm::vec3 point);
	void AddPoint(glm::vec3 point);
	void Clear();

	glm::vec3& GetMinPos() { return minPos; }
	glm::vec3& GetMaxPos() { return maxPos; }
	std::pair<glm::vec3, glm::vec3>& GetExtents() { return std::make_pair(minPos, maxPos); }

	void SetMinPos(glm::vec3 newPosition) { minPos = newPosition; }
	void SetMaxPos(glm::vec3 newPosition) { maxPos = newPosition; }
	void SetExtents(glm::vec3 newMinPos, glm::vec3 newMaxPos);

	std::shared_ptr<LineMesh> GetAABBLines(glm::vec3 color);

private:
	glm::vec3 minPos;
	glm::vec3 maxPos;
	std::shared_ptr<LineMesh> aabbMesh = nullptr;
};