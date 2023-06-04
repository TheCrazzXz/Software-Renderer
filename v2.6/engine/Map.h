#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include "../graphical_library/maths/maths.h"
#include "../graphical_library/maths/vec.h"

#include "entity.h"

#include "shapes.h"
#include "Camera.h"
#include "../graphical_library/memory/vector.h"

bool worldMap[16 * 16] = 
{
    1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,   
    1, 0, 0, 0,  1, 0, 0, 1,  0, 0, 0, 0,  0, 0, 0, 1,   
    1, 0, 0, 0,  1, 0, 0, 1,  0, 0, 0, 0,  0, 0, 0, 1,  
    1, 0, 0, 0,  1, 0, 0, 1,  0, 0, 1, 1,  1, 0, 0, 1,   
    
    1, 0, 0, 0,  1, 0, 0, 1,  0, 0, 1, 0,  0, 0, 0, 1,  
    1, 0, 0, 0,  1, 1, 1, 1,  0, 0, 1, 0,  0, 0, 0, 1,   
    1, 0, 0, 0,  1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 
    1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 
    
    1, 0, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1,  
    1, 0, 0, 1,  1, 1, 1, 0,  0, 0, 1, 1,  1, 1, 1, 1,  
    1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 1,  
    1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 1,   
    
    1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 1,   
    1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 1,  
    1, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 0, 0, 1,  
    1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,   
    
    
};

void mapToCubes(Vector* cubes, bool* map, ulong width, ulong height, ulong worldHeight)
{
    Vector3 position = {0, 0, 0};
    Vector3 scale = {1, 1, 1};
    Vector3 rotation = {0, 0, 0};
    for(int x = 0 ; x < width ; x += 1)
    {
        for(int z = 0 ; z < height  ; z += 1)
        {
            
            if(map[z * width + x] == 1)
            {
                for(int y = 0 ; y < worldHeight ; y += 1)
                {
                    position.x = x * 2;
                    position.z = z * 2;
                    position.y = y * 2;
                    
                    Entity cube = createEntity(cubeModel, position, rotation, scale, 0xff0000ff, 0xff0000ff, 0xff0000ff);
                    Vector_Push_back(cubes, &cube);
                }
                
            }
            else
            {
                position.x = x * 2;
                position.z = z * 2;
                position.y = worldHeight + 2;
                Entity cube = createEntity(cubeModel, position, rotation, scale, 0xff0000ff, 0xff0000ff, 0xff0000ff);
                Vector_Push_back(cubes, &cube);
            }
            
        }
    }
}

#endif