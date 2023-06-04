#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../graphical_library/maths/vec.h"
#include "../graphical_library/maths/maths.h"

struct Camera
{
	Vector3 position;
	float cameraYaw;
	float cameraPitch;
	float speed;
	float mouseSensibility;
};

void Camera_getForward(struct Camera* camera, float* outX, float* outZ)
{
	Vector3 forward = {-sin(DEGREE_TO_RADIANS(camera->cameraYaw)), 1.0,  cos(DEGREE_TO_RADIANS(camera->cameraYaw))};
	//printVector3(&forward);
	
	*outX = forward.x * camera->mouseSensibility;
	*outZ = forward.z * camera->mouseSensibility;
}

typedef struct Camera Camera;

#endif