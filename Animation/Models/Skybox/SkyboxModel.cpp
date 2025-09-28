#include "SkyboxModel.h"

void SkyboxModel::Initialization()
{
	// Hardcoded info no need to extract or update the info
	skyboxVertex.vertices.resize(36);

	/* front */
	skyboxVertex.vertices[0].position = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[1].position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[2].position = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[3].position = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[4].position = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[5].position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	/* back */
	skyboxVertex.vertices[6].position = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[7].position = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[8].position = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[9].position = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[10].position = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[11].position = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);

	/* left */
	skyboxVertex.vertices[12].position = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[13].position = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[14].position = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[15].position = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[16].position = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[17].position = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);

	/* right */
	skyboxVertex.vertices[18].position = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[19].position = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[20].position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[21].position = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[22].position = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[23].position = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);

	/* top */
	skyboxVertex.vertices[24].position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[25].position = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[26].position = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[27].position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[28].position = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[29].position = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);

	/* bottom */
	skyboxVertex.vertices[30].position = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[31].position = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[32].position = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[33].position = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
	skyboxVertex.vertices[34].position = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	skyboxVertex.vertices[35].position = glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);

	DEV_LOG(TE_INFO, "SkyboxModel", "Vertices loaded!");
}