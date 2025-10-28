#include "Node.h"

Node::Node(std::string newName)
{
	nodeName = newName;
	childs = {};
	parent = {};
	localTRSMatrix = glm::mat4(1.f);
	parentMatrix = glm::mat4(1.f);
	translation = glm::vec3(1.f);
	rotation = glm::identity<glm::quat>();
	scaling = glm::vec3(1.f);
	translationMatrix = glm::mat4(1.f);
	rotationMatrix = glm::mat4(1.f);
	scalingMatrix = glm::mat4(1.f);
	rootTransformMatrix = glm::mat4(1.f);
}

void Node::AddChildNode(std::string child)
{
	std::shared_ptr<Node> newChild = std::make_shared<Node>(child);
	newChild->parent = shared_from_this();
	childs.push_back(newChild);
}

void Node::AddChildNodes(std::vector<std::string> childs)
{
	for (std::string& child : childs)
	{
		std::shared_ptr<Node> newChild = std::make_shared<Node>(child);
		newChild->parent = shared_from_this();
		this->childs.push_back(newChild);
	}
}

void Node::SetTranslation(glm::vec3 newTranslation)
{
	translation = newTranslation;
	translationMatrix = glm::translate(glm::mat4(1.f), translation);
}

void Node::SetRotation(glm::quat newRotation)
{
	rotation = newRotation;
	rotationMatrix = glm::mat4_cast(rotation);
}

void Node::SetScaling(glm::vec3 newScaling)
{
	scaling = newScaling;
	scalingMatrix = glm::scale(glm::mat4(1.f), scaling);
}

void Node::UpdateTRSMatrix()
{
	if (std::shared_ptr<Node> parentNode = parent.lock())
	{
		parentMatrix = parentNode->GetRTSMatrix();
	}
	else
	{
		parentMatrix = glm::mat4(1.f);
	}

	localTRSMatrix = rootTransformMatrix * parentMatrix * translationMatrix * rotationMatrix * scalingMatrix;
}

std::vector<std::string> Node::GetChildNames()
{
	return std::vector<std::string>();
}
