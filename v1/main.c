#define WINDOW_WIDTH_CONSOLE 150
#define WINDOW_HEIGHT_CONSOLE 42

#define WINDOW_WIDTH_WINDOW 800
#define WINDOW_HEIGHT_WINDOW 640

const float FOV = 70.0;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1000.0;

#define DEGREE_TO_RADIANS(a) (2 * M_PI * a / 360) // convert degrees to radians

#include "sh/PixelBuffer.h" 
#include "sh/Drawer.h"
#include "sh/TerminalRenderer.h"

#include "maths/matrix.h"
#include "maths/matrix4.h"
#include "maths/matrix_transform.h"

#include "maths/projection.h"
#include "maths/maths.h"

#include "cube.h"

/*
	WINDOW_ENABLE = 0 : Don't use SDL, don't include it and don't use any window, render on console
	WINDOW_ENABLE = 1 : Import SDL from <SDL2/SDL.h> and use a window to render	

*/
#define WINDOW_ENABLE 0

#if WINDOW_ENABLE == 0

#define WINDOW_WIDTH WINDOW_WIDTH_CONSOLE
#define WINDOW_HEIGHT WINDOW_HEIGHT_CONSOLE

#else

#define WINDOW_WIDTH WINDOW_WIDTH_WINDOW
#define WINDOW_HEIGHT WINDOW_HEIGHT_WINDOW
#include <SDL2/SDL.h>

#endif

Vector3 camera = {0.0, 0.0, 10.0};
float cameraPitch = 0.0f;
float cameraYaw = 0.0f;

void update(PixelBuffer* pixelBuffer, float angle)
{
	/*
		A     B
		*-----*
		|     |
		|     |
		*-----*
		C     D

		A( -0.5, 0.5 ) B(0.5, 0.5)
		C(-0.5, -0.5) D(0.5, -0.5)

	*/

	/*float cube_vertices[] = {
		-1, -1, -1,
		1, -1, -1,
		-1, 1, -1,
		1, 1, -1,
		-1, -1, 1,
		1, -1, 1,
		-1, 1, 1,
		1, 1, 1
	};*/

	

	float z = 0;
	Vector3 pos = {0, 0, 0};
	Vector3 pos1 = {2, 0, 0};
	Vector3 rotation = {0, 0, 0};
	Vector3 scale = {0.25, 0.25, 3};

	
	Matrix4 viewMatrix = createViewMatrix(camera, cameraYaw, cameraPitch);

	//printf("Camera z : %f\n", camera.z);

	//pos.x /= pow(2, pos.z);
	//pos.y /= pow(2, pos.z);
	//pos.z /= pow(2, pos.z);
	//printVector3(&scale);

	//transformationMatrix = createTransformationMatrix(pos, rotation, scale);
		
	flatPlane(pos, rotation, scale, &viewMatrix, pixelBuffer);
	cube(pos1, rotation, scale, &viewMatrix, pixelBuffer);

	//printf("Camera z : %f\n", camera.z);
}

int main(int argc, char ** argv)
{
	createProjectionMatrix(DEGREE_TO_RADIANS(FOV), (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	PixelBuffer pixelBuffer = createPixelBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 0x00000000);
	
	/*clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
    printf("Updating...\n");
    update(&pixelBuffer);
	printf("Ended updating\n");
	*/
	//vertices2Color(pixelRGB(255, 255, 255), pixelRGB(255, 255, 255));
	float angle = 45;

	clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
	update(&pixelBuffer, angle);

	#if (WINDOW_ENABLE == 0)
	while(1)
	{
		
		angle += 1.0f;
		clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
	    update(&pixelBuffer, angle);
	    renderPixelBufferTerminal(&pixelBuffer, false);

	    char buffer;

	    buffer = fgetc (stdin);

	    if(buffer == 'z')
	    {
	    	camera.z -= 0.1;
	    }
	    else if(buffer == 's')
	    {
	    	camera.z += 0.1;
	    }
	    else if(buffer == 'q')
	    {
	    	camera.x -= 0.1;
	    }
	    else if(buffer == 'd')
	    {
	    	camera.x += 0.1;
	    }

	    system("clear");
	}
	#endif
	

    #if (WINDOW_ENABLE == 1)
	    int quit = 0;
	    
	    
	    SDL_Event event;
	 
	    SDL_Init(SDL_INIT_VIDEO);
	 
	    SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
	        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	 
	    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	    SDL_Texture * texture = SDL_CreateTexture(renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	
	
		int add = 0;

		int leftMouseButtonDown = 0;

		SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));

	    while (!quit)
	    {
	    	angle++;
	    	
	        SDL_WaitEvent(&event);
	 		
	        bool shouldUpdate = false;

	        if(event.type == SDL_QUIT)
	        {
	                quit = 1;
	        }
	        else if(event.type == SDL_KEYDOWN)
	        {
	        	shouldUpdate = true;
	        	switch (event.key.keysym.sym) {
			        case SDLK_UP:
			            camera.z -= 0.1f;
			            break;
			        case SDLK_DOWN:
			            camera.z += 0.1f;
			            break;
			        case SDLK_RIGHT:
			            camera.x += 0.1f;
			            break;
			        case SDLK_SPACE:
			            camera.y += 0.1f;
			            break;
			        case SDLK_LSHIFT:
			            camera.y -= 0.1f;
			            break;
			        case SDLK_LEFT:
			            camera.x -= 0.1f;
			            break;

			        case SDLK_KP_4:
			        	cameraYaw -= 1.0f;
			        	break;
			        case SDLK_KP_6:
			        	cameraYaw += 1.0f;
			        	break;
			        case SDLK_KP_2:
			        	cameraPitch -= 1.0f;
			        	break;
			        case SDLK_KP_8:
			        	cameraPitch += 1.0f;
			        	break;
    			}
	        }
	            	

	        if(shouldUpdate)
	        {
	        	clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
	    		update(&pixelBuffer, angle);
	        	SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));
	        }
	        


	        
			SDL_RenderCopyEx(
				renderer,
				texture,
				NULL,
				NULL,
				0.0,
				NULL,
				SDL_FLIP_VERTICAL
			);

			SDL_RenderPresent(renderer);
			add++;
	    }

	    free(pixelBuffer.buffer);
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
	 
	    SDL_DestroyWindow(window);
	    SDL_Quit();
 	
	#endif

    return 0;
    
}