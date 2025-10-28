#include "PlaneModel.h"


PlaneModel::PlaneModel(const float height, const float width, const glm::vec3 color)
{
	this->height = height;
	this->width = width;
	this->color = color;
	Initialization();
}

void PlaneModel::Initialization()
{

}