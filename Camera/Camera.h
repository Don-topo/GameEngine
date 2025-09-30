#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>
#include "../Logger/Logger.h"

enum class cameraType : uint8_t {
	free = 0,
	firstPerson,
	thirdPerson,
	stationary,
	stationaryFollowing
};

enum class cameraProjection : uint8_t {
	perspective = 0,
	orthogonal
};

struct CameraSettings {
	std::string csCamName = "Camera";

	glm::vec3 csWorldPosition = glm::vec3(0.0f);
	float csViewAzimuth = 0.0f;
	float csViewElevation = 0.0f;

	int csFieldOfView = 90;
	float csOrthoScale = 20.0f;

	bool csFirstPersonLockView = true;
	int csFirstPersonBoneToFollow = 0;
	glm::vec3 csFirstPersonOffsets = glm::vec3(0.0f);

	float csThirdPersonDistance = 5.0f;
	float csThirdPersonHeightOffset = 0.0f;

	float csFollowCamHeightOffset = 2.0f;

	cameraType csCamType = cameraType::free;
	cameraProjection csCamProjection = cameraProjection::perspective;
};

class Camera
{
public:
    std::string getName() { return mCamSettings.csCamName; }
    void setName(std::string name) { mCamSettings.csCamName = name; }

    void updateCamera(glm::vec3 upMovement, glm::vec3 rightMovement, glm::vec3 forwardMovement, const float deltaTime);
    void moveCameraTo(glm::vec3 position);

    glm::vec3 getWorldPosition() { return mCamSettings.csWorldPosition; }
    void setWorldPosition(glm::vec3 position) { mCamSettings.csWorldPosition = position; }
    float getAzimuth() { return mCamSettings.csViewAzimuth; }
    void setAzimuth(float azimuth) { mCamSettings.csViewAzimuth = azimuth; }
    float getElevation() { return mCamSettings.csViewElevation; }
    void setElevation(float elevation) { mCamSettings.csViewElevation = elevation; }

    glm::mat4 getViewMatrix();

    float getFOV() { return mCamSettings.csFieldOfView; }
    void setFOV(float fieldOfView) { mCamSettings.csFieldOfView = fieldOfView; }

    /* glm::ortho lets us scale the resulting view by scaling all values */
    float getOrthoScale() { return mCamSettings.csOrthoScale; }
    void setOrthoScale(float scale) { mCamSettings.csOrthoScale = scale; }

    cameraType getCameraType() { return mCamSettings.csCamType; }
    void setCameraType(cameraType type) { mCamSettings.csCamType = type; }

    cameraProjection getCameraProjection() { return mCamSettings.csCamProjection; }
    void setCameraProjection(cameraProjection proj) { mCamSettings.csCamProjection = proj; }

    //std::shared_ptr<AssimpInstance> getInstanceToFollow();
    //void setInstanceToFollow(std::shared_ptr<AssimpInstance> instance);
    //void clearInstanceToFollow();
    //std::vector<std::string> getBoneNames();
    //void setBoneMatrix(glm::mat4 matrix);

    CameraSettings getCameraSettings() { return mCamSettings; }
    void setCameraSettings(CameraSettings settings) { mCamSettings = settings; }

private:
    void updateCameraView(const float deltaTime);
    void updateCameraPosition(glm::vec3 upMovement, glm::vec3 rightMovement, glm::vec3 forwardMovement, const float deltaTime);

    CameraSettings mCamSettings = {};

    glm::vec3 mViewDirection = glm::vec3(0.0f);
    glm::vec3 mRightDirection = glm::vec3(0.0f);
    glm::vec3 mUpDirection = glm::vec3(0.0f);

    /* world up is positive Y */
    const glm::vec3 mWorldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    /* vector pointing to the z axis to have an Azimuth reference */
    const glm::vec3 mSideVector = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::mat4 mFirstPersonBoneMatrix = glm::mat4(1.0f);
    std::vector<std::string> mFirstPersonBoneNames{};
};