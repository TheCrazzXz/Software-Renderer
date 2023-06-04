#pragma once

#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL2/SDL.h>
#include "../graphical_library/maths/vec.h"
#include <stdbool.h>
#include "Camera.h"

#define CAN_FLY true

bool Mousefirst  = true;
bool MouseMotion = false;

float MouseRelX = 0;
float MouseRelY = 0;

Vector3 vec;
bool passMode = false;

void windowKeyboardCameraInput(SDL_Event event, Camera* camera, Vector3* displacement)
{
	*displacement = Vec3_create(0, 0, 0);
	float x;
	float z;

	Camera_getForward(camera, &x, &z);

	switch (event.key.keysym.sym)
	{
		case SDLK_p:
        	if(passMode == true)
        	{
        		passMode = false;
        	}
        	else
        	{
        		passMode = true;
        	}
        	break;
        case SDLK_UP:
        	vec = Vec3_create(-x, 0, -z);
            *displacement = Vectors3_add(displacement, &vec);
            break;
        case SDLK_DOWN:
            vec = Vec3_create(x, 0, z);
            *displacement = Vectors3_add(displacement, &vec);
            break;
        case SDLK_RIGHT:
        	vec = Vec3_create(z, 0, -x);
            *displacement = Vectors3_add(displacement, &vec);
            break;
        case SDLK_LEFT:
        	vec = Vec3_create(-z, 0, x);
            *displacement = Vectors3_add(displacement, &vec);
            break;

        case SDLK_SPACE:
            if(CAN_FLY)
            {
            	vec = Vec3_create(0, camera->speed, 0);
            	*displacement = Vectors3_add(displacement, &vec);
            }
            
            break;
        case SDLK_w:
        	if(CAN_FLY)
        	{
        		vec = Vec3_create(0, -camera->speed, 0);
            	*displacement = Vectors3_add(displacement, &vec);
        	}
        	
            break;
        
        case SDLK_ESCAPE:
        	SDL_SetRelativeMouseMode(SDL_FALSE);
        	break;
        case SDLK_KP_4:
        	camera->cameraYaw -= 3.0f;
        	break;
        case SDLK_KP_6:
        	camera->cameraYaw += 3.0f;
        	break;
        case SDLK_KP_2:
        	camera->cameraPitch += 3.0f;
        	break;
        case SDLK_KP_8:
        	camera->cameraPitch -= 3.0f;
        	break;
        default:
        	break;

	}
	*displacement = Vec3_mulNumber_get(displacement, camera->speed);
}

void windowMouseInput(SDL_Event event, Camera* camera)
{
	if(event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	// Mouse moved ?
	  MouseMotion = true;
	  if (!Mousefirst) {
	    MouseRelX = event.motion.xrel;
	    MouseRelY = event.motion.yrel;
	  } else {
	    Mousefirst = false;
	    MouseRelX = 0;
	    MouseRelY = 0;
	  }
	if (MouseMotion) {
	  MouseMotion = false;
	  camera->cameraYaw += MouseRelX * 1;
	  camera->cameraPitch += MouseRelY * 1;
	  camera->cameraPitch = clampFloat(camera->cameraPitch, -90, 90);
	  

	}
}

#endif