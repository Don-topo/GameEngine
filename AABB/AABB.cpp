#include "AABB.h"

AABB::AABB()
{
	aabbMesh = std::make_shared<LineMesh>();
	aabbMesh->vertices.resize(24);

	Clear();
}

void AABB::Create(glm::vec3 point)
{
	minPos = point;
	maxPos = point;
}

void AABB::AddPoint(glm::vec3 point)
{
	minPos.x = std::min(minPos.x, point.x);
	minPos.y = std::min(minPos.y, point.y);
	minPos.z = std::min(minPos.z, point.z);

	maxPos.x = std::max(maxPos.x, point.x);
	maxPos.y = std::max(maxPos.y, point.y);
	maxPos.z = std::max(maxPos.z, point.z);
}

void AABB::Clear()
{
	minPos = glm::vec3(std::numeric_limits<float>::max());
	maxPos = glm::vec3(-std::numeric_limits<float>::max());
}

void AABB::SetExtents(glm::vec3 newMinPos, glm::vec3 newMaxPos)
{
	minPos = newMinPos;
	maxPos = newMaxPos;
}

std::shared_ptr<LineMesh> AABB::GetAABBLines(glm::vec3 color)
{
	aabbMesh->vertices.at(0) = { {minPos.x, minPos.y, minPos.z}, color };
	aabbMesh->vertices.at(1) = { {maxPos.x, minPos.y, minPos.z}, color };

	aabbMesh->vertices.at(2) = { {minPos.x, minPos.y, minPos.z}, color };
	aabbMesh->vertices.at(3) = { {minPos.x, maxPos.y, minPos.z}, color };

	aabbMesh->vertices.at(4) = { {maxPos.x, maxPos.y, minPos.z}, color };
	aabbMesh->vertices.at(5) = { {maxPos.x, minPos.y, minPos.z}, color };

	aabbMesh->vertices.at(6) = { {maxPos.x, maxPos.y, minPos.z}, color };
	aabbMesh->vertices.at(7) = { {minPos.x, maxPos.y, minPos.z}, color };

	aabbMesh->vertices.at(8) = { {minPos.x, minPos.y, maxPos.z}, color };
	aabbMesh->vertices.at(9) = { {maxPos.x, minPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(10) = { {minPos.x, minPos.y, maxPos.z}, color };
	aabbMesh->vertices.at(11) = { {minPos.x, maxPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(12) = { {maxPos.x, maxPos.y, maxPos.z}, color };
	aabbMesh->vertices.at(13) = { {maxPos.x, minPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(14) = { {maxPos.x, maxPos.y, maxPos.z}, color };
	aabbMesh->vertices.at(15) = { {minPos.x, maxPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(16) = { {minPos.x, minPos.y, minPos.z}, color };
	aabbMesh->vertices.at(17) = { {minPos.x, minPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(18) = { {maxPos.x, minPos.y, minPos.z}, color };
	aabbMesh->vertices.at(19) = { {maxPos.x, minPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(20) = { {minPos.x, maxPos.y, minPos.z}, color };
	aabbMesh->vertices.at(21) = { {minPos.x, maxPos.y, maxPos.z}, color };

	aabbMesh->vertices.at(22) = { {maxPos.x, maxPos.y, minPos.z}, color };
	aabbMesh->vertices.at(23) = { {maxPos.x, maxPos.y, maxPos.z}, color };

	return aabbMesh;
}