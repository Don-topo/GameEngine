#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

void Camera::updateCamera(glm::vec3 upMovement, glm::vec3 rightMovement, glm::vec3 forwardMovement, const float deltaTime)
{
	if (deltaTime == 0.f)
	{
		return;
	}
    /* no camera movement on stationary cam */
    if (mCamSettings.csCamType == cameraType::stationary) {
        return;
    }

    /* default handling is free camera if nothing has been locked  */
    //if (!mCamSettings.csInstanceToFollow.lock()) {
    //    updateCameraView(renderData, deltaTime);
    //    updateCameraPosition(renderData, deltaTime);
    //    return;
    //}

    /* follow the locked object depending on the camera type */
    switch (mCamSettings.csCamType) {
    case cameraType::firstPerson:
    {
        mCamSettings.csWorldPosition = mFirstPersonBoneMatrix[3];

        if (mCamSettings.csFirstPersonLockView) {
            /* get elevation */
            glm::vec3 elevationVector = glm::mat3(mFirstPersonBoneMatrix) * mSideVector;
            mCamSettings.csViewElevation = glm::degrees(std::atan2(glm::length(glm::cross(elevationVector, mWorldUpVector)),
                glm::dot(elevationVector, -mWorldUpVector))) - 90.0f;

            /* get azimuth */
            glm::vec3 azimuthVector = glm::mat3(mFirstPersonBoneMatrix) * mSideVector;
            /* we are only interested in the rotation angle around the vertical axis */
            azimuthVector.y = 0.0f;
            mCamSettings.csViewAzimuth = glm::degrees(std::acos(glm::dot(glm::normalize(azimuthVector), -mSideVector)));
            /* support full 360 degree for Azimuth */
            if (azimuthVector.x < 0.0f) {
                mCamSettings.csViewAzimuth = 360.0f - mCamSettings.csViewAzimuth;
            }
        }

        updateCameraView(deltaTime);
    }
    break;
    case cameraType::thirdPerson:
    {
        //std::shared_ptr<AssimpInstance> instance = mCamSettings.csInstanceToFollow.lock();
        //InstanceSettings instSettings = instance->getInstanceSettings();

        //float rotationAngle = 180.0f - instSettings.isWorldRotation.y;
        //mCamSettings.csViewAzimuth = rotationAngle;

        //glm::vec3 offset = glm::vec3(-std::sin(glm::radians(rotationAngle)), 1.0f, std::cos(glm::radians(rotationAngle))) * mCamSettings.csThirdPersonDistance;
        //offset.y += mCamSettings.csThirdPersonHeightOffset;
        //mCamSettings.csWorldPosition = instSettings.isWorldPosition + offset;

        //glm::vec3 viewDirection = instSettings.isWorldPosition - mCamSettings.csWorldPosition;
        //mCamSettings.csViewElevation = (90.0f - glm::degrees(std::acos(glm::dot(glm::normalize(viewDirection), mWorldUpVector)))) / 2.0f;

        //updateCameraView(deltaTime);
    }
    break;
    case cameraType::stationaryFollowing:
    {
        //std::shared_ptr<AssimpInstance> instance = mCamSettings.csInstanceToFollow.lock();
        //InstanceSettings instSettings = instance->getInstanceSettings();

        //glm::vec3 viewDirection = instance->getWorldPosition() - mCamSettings.csWorldPosition + glm::vec3(0.0f, mCamSettings.csFollowCamHeightOffset, 0.0f);

        //mCamSettings.csViewElevation = 90.0f - glm::degrees(std::acos(glm::dot(glm::normalize(viewDirection), mWorldUpVector)));

        /* map to 'y = 0' to avoid elevation angle taking over for the largest angle */
        //float rotateAngle = glm::degrees(std::acos(glm::dot(glm::normalize(glm::vec3(viewDirection.x, 0.0f, viewDirection.z)), glm::vec3(0.0f, 0.0f, -1.0f))));
        /* support full 360 degree for Azimuth */
        //if (viewDirection.x < 0.0f) {
            //rotateAngle = 360.0f - rotateAngle;
        //}
        //mCamSettings.csViewAzimuth = rotateAngle;

        //updateCameraView(deltaTime);
    }
    break;
    default:
        break;
    }
}

void Camera::moveCameraTo(glm::vec3 position)
{
	mCamSettings.csWorldPosition = position;
	/* hard-code values for now, reversing from lookAt() matrix is too much work */
	mCamSettings.csViewAzimuth = 310.0f;
	mCamSettings.csViewElevation = -15.0f;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(mCamSettings.csWorldPosition, mCamSettings.csWorldPosition + mViewDirection, mUpDirection);
}

void Camera::updateCameraView(const float deltaTime)
{
	float azimRad = glm::radians(mCamSettings.csViewAzimuth);
	float elevRad = glm::radians(mCamSettings.csViewElevation);

	float sinAzim = std::sin(azimRad);
	float cosAzim = std::cos(azimRad);
	float sinElev = std::sin(elevRad);
	float cosElev = std::cos(elevRad);

	/* update view direction */
	mViewDirection = glm::normalize(glm::vec3(
		sinAzim * cosElev, sinElev, -cosAzim * cosElev));

	/* calculate right and up direction */
	mRightDirection = glm::normalize(glm::cross(mViewDirection, mWorldUpVector));
	mUpDirection = glm::normalize(glm::cross(mRightDirection, mViewDirection));
}

void Camera::updateCameraPosition(glm::vec3 upMovement, glm::vec3 rightMovement, glm::vec3 forwardMovement, const float deltaTime)
{
	mCamSettings.csWorldPosition +=
		forwardMovement * deltaTime * mViewDirection +
		rightMovement * deltaTime * mRightDirection +
		upMovement * deltaTime * mUpDirection;
}
