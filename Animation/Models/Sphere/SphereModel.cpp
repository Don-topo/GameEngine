#include "SphereModel.h"

void SphereModel::Initialization()
{
    std::vector<std::vector<glm::vec3>> constructionVertexData;

    /* construct rings with position data */
    for (unsigned int i = 1; i < vertDiv; ++i) {
        /* divide the elevation value into radians - we start at 90 degree! */
        float elevation = glm::radians(90.0f) - glm::radians(180.0f / vertDiv) * i;
        std::vector<glm::vec3> ringData{};

        for (unsigned int j = 0; j < hortDiv; ++j) {
            float amzimuth = glm::radians(360.0f / hortDiv) * j;

            ringData.emplace_back(glm::vec3(
                std::sin(amzimuth) * std::cos(elevation) * sphereRadius,
                std::sin(elevation) * sphereRadius,
                std::cos(amzimuth) * std::cos(elevation) * sphereRadius
            ));
        }
        constructionVertexData.push_back(ringData);
    }

    /* construct top triangle ring */
    glm::vec3 topPos = glm::vec3(0.0f, sphereRadius, 0.0f);
    for (unsigned int j = 0; j < hortDiv; ++j) {
        sphereMesh.vertices.emplace_back(topPos, color);
        sphereMesh.vertices.emplace_back(constructionVertexData.at(0).at(j), color);

        sphereMesh.vertices.emplace_back(constructionVertexData.at(0).at(j), color);
        sphereMesh.vertices.emplace_back(constructionVertexData.at(0).at((j + 1) % hortDiv), color);

        sphereMesh.vertices.emplace_back(constructionVertexData.at(0).at((j + 1) % hortDiv), color);
        sphereMesh.vertices.emplace_back(topPos, color);
    }

    /* 2x triangles as quad, over all rings */
    for (unsigned int i = 0; i < vertDiv - 2; ++i) {
        for (unsigned int j = 0; j < hortDiv; ++j) {
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at(j), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at(j), color);

            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at(j), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at((j + 1) % hortDiv), color);

            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at((j + 1) % hortDiv), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at(j), color);


            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at((j + 1) % hortDiv), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at(j), color);

            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at(j), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at((j + 1) % hortDiv), color);

            sphereMesh.vertices.emplace_back(constructionVertexData.at(i + 1).at((j + 1) % hortDiv), color);
            sphereMesh.vertices.emplace_back(constructionVertexData.at(i).at((j + 1) % hortDiv), color);
        }
    }

    /* construct bottom */
    glm::vec3 bottomPos = glm::vec3(0.0f, -sphereRadius, 0.0f);
    for (unsigned int j = 0; j < hortDiv; ++j) {
        sphereMesh.vertices.emplace_back(constructionVertexData.at(vertDiv - 2).at((j + 1) % hortDiv), color);
        sphereMesh.vertices.emplace_back(constructionVertexData.at(vertDiv - 2).at(j), color);

        sphereMesh.vertices.emplace_back(constructionVertexData.at(vertDiv - 2).at(j), color);
        sphereMesh.vertices.emplace_back(bottomPos, color);

        sphereMesh.vertices.emplace_back(bottomPos, color);
        sphereMesh.vertices.emplace_back(constructionVertexData.at(vertDiv - 2).at((j + 1) % hortDiv), color);
    }

    DEV_LOG(TE_INFO, "SphereModel", "Vertices created!");
}