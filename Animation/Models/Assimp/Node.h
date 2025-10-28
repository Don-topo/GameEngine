#pragma once

#include "../../../Logger/Logger.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node(std::string newName);
	void AddChildNode(std::string child);
	void AddChildNodes(std::vector<std::string> childs);
	void SetTranslation(glm::vec3 newTranslation);
	void SetRotation(glm::quat newRotation);
	void SetScaling(glm::vec3 newScaling);
	void SetRootTransformationMatrix(glm::mat4 newTransformationMatrix) { rootTransformMatrix = newTransformationMatrix; }
	void UpdateTRSMatrix();

	std::string GetNodeName() { return nodeName; }
	std::shared_ptr<Node> GetParentNode() { parent; }
	std::string GetParentNodeName() { return parent.lock()->GetNodeName(); }
	std::vector<std::shared_ptr<Node>> GetChildNodes() { return childs; }
	std::vector<std::string> GetChildNames();
	glm::mat4 GetRTSMatrix() { return localTRSMatrix; }

private:
	std::string nodeName;
	std::weak_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> childs;

	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scaling;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scalingMatrix;

	glm::mat4 parentMatrix;
	glm::mat4 localTRSMatrix;

	glm::mat4 rootTransformMatrix;
};