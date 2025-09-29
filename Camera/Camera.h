#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "../Logger/Logger.h"

class Camera
{
public:
	std::string GetName() { return cameraName; }
	void SetName(std::string newName) { cameraName = newName; };

	void UpadeCamera();
	void MoveCameraToPosition();

	glm::vec3 GetWorldPosition() { return worldPosition; }

private:
	std::string cameraName;
	glm::vec3 worldPosition;

};