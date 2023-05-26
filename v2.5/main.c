/*
	main.c : Software renderer version 2.5

	- Added line clipping in window.
	- Added support for rendering data with shaders.
	- Fixed graphical problems / crashing problems (a lot).
	- Now support for filling triangles ! (this was the hardest thing to implement lmao).
	Use shSetWireframeMode(true) before rendering anything to enable wireframe (not filling triangles).

	In engine (nothing to do with graphical library, i've created it just to test) :
	- Entities are added, so instances of objects.
	You can set objects position, rotation, scale, and two colors
	Colors will be interpolated...
	But : Note that for now if you don't use uniform colors, so different entity.start_color and entity.end_color,
	The program might slow, mostly if you get closer to objects.

	You should get a nice green rotating cube.
	Happy Halloween !

	Written by TheCrazzXz 
		- 26 of May, 2023
*/

#define WINDOW_WIDTH_CONSOLE 150
#define WINDOW_HEIGHT_CONSOLE 42

#define WINDOW_WIDTH_WINDOW 800
#define WINDOW_HEIGHT_WINDOW 640

/*
	WINDOW_ENABLE = 0 : Don't use SDL, don't include it and don't use any window, render on console
	WINDOW_ENABLE = 1 : Import SDL from <SDL2/SDL.h> and use a window to render	

	USING_ANDROID_PHONE : If using an android phone, use input from it 

*/

#define USING_ANDROID_PHONE 0

/*
	ANDROID_PHONE_SCREEN_MODE : If using android phone

	0 : Vertical (h > w)
	1 : Horizontal (w > h)
	1337 : ??

*/

#define ANDROID_PHONE_SCREEN_MODE 1

#if ANDROID_PHONE_SCREEN_MODE == 0

#define ANDROID_PHONE_SCREEN_WIDTH 1400 
#define ANDROID_PHONE_SCREEN_HEIGHT 620 

#else

#define ANDROID_PHONE_SCREEN_WIDTH 620 
#define ANDROID_PHONE_SCREEN_HEIGHT 1400 

#endif


#define WINDOW_ENABLE 1

#if WINDOW_ENABLE == 0

#define WINDOW_WIDTH WINDOW_WIDTH_CONSOLE
#define WINDOW_HEIGHT WINDOW_HEIGHT_CONSOLE

#else

#if USING_ANDROID_PHONE == 1

#define WINDOW_WIDTH ANDROID_PHONE_SCREEN_WIDTH
#define WINDOW_HEIGHT ANDROID_PHONE_SCREEN_HEIGHT

#else

#define WINDOW_WIDTH WINDOW_WIDTH_WINDOW
#define WINDOW_HEIGHT WINDOW_HEIGHT_WINDOW

#endif
#include <SDL2/SDL.h>

#endif

const float FOV = 70.0;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1000.0;

#define SPEED 0.5


#define DEGREE_TO_RADIANS(a) (2 * M_PI * a / 360) // convert degrees to radians

#include "graphical_library/sh/PixelBuffer.h" 
#include "graphical_library/sh/Drawer.h"
#include "graphical_library/sh/TerminalRenderer.h"

#include "graphical_library/maths/matrix.h"
#include "graphical_library/maths/matrix4.h"
#include "graphical_library/maths/matrix_transform.h"

#include "graphical_library/maths/projection.h"
#include "graphical_library/maths/maths.h"

#include "engine/entity.h"

#include "engine/shapes.h"

int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

#define BACKGROUND_COLOR pixelRGB(0, 0, 0)

Vector3 camera = {0.0, 0.5, 20};
float cameraPitch = 0.0f;
float cameraYaw = 0.0f;

Vector flatPlanes;
Vector cubes;


float angle = 0;

void update(PixelBuffer* pixelBuffer, Matrix4* viewMatrix)
{
	((Entity*)Vector_element(&cubes, 0))->rotation.z++;
	((Entity*)Vector_element(&cubes, 0))->rotation.x += 0.5;
	((Entity*)Vector_element(&cubes, 0))->rotation.y += 0.25;

	Entity_render(Vector_element(&cubes, 0), viewMatrix, pixelBuffer, &camera);

}

int main(int argc, char ** argv)
{
	//shSetWireframeMode(true);
	Matrix4 viewMatrix = createViewMatrix(camera, cameraYaw, cameraPitch);
	flatPlanes = Vector_create(sizeof(Entity));
	cubes = Vector_create(sizeof(Entity));
	createProjectionMatrix(DEGREE_TO_RADIANS(FOV), (float)window_width/(float)window_height, NEAR_PLANE, FAR_PLANE);
	PixelBuffer pixelBuffer = createPixelBuffer(window_width, window_height, BACKGROUND_COLOR);

	Vector3 position = {0, 0, 0};
	Vector3 rotation = {0, 0, 0};
	Vector3 scale = {2, 2, 2};

	Entity cube = createEntity(cubeModel, position, rotation, scale, 0xff00ff00, 0xff00ff00);
	Vector_Push_back(&cubes, &cube);

	#if (WINDOW_ENABLE == 0)
	while(1)
	{
		clearColor(&pixelBuffer, BACKGROUND_COLOR);
	    update(&pixelBuffer, &viewMatrix);
	    renderPixelBufferTerminal(&pixelBuffer, false);

	    char buffer;

	    buffer = fgetc (stdin);

	    if(buffer == 'z')
	    {
	    	camera.z -= SPEED;
	    }
	    else if(buffer == 's')
	    {
	    	camera.z += SPEED;
	    }
	    else if(buffer == 'q')
	    {
	    	camera.x -= SPEED;
	    }
	    else if(buffer == 'd')
	    {
	    	camera.x += SPEED;
	    }

	    system("clear");
	}
	#endif
	

    #if (WINDOW_ENABLE == 1)
	    int quit = 0;
	    
	    
	    SDL_Event event;
	 
	    SDL_Init(SDL_INIT_VIDEO);
	 
	    SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
	        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_RESIZABLE);
	 
	    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	    SDL_Texture * texture = SDL_CreateTexture(renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

	
	
		int add = 0;

		int leftMouseButtonDown = 0;
		int counter = 0;

		clock_t beforeAtBegginingOfTheIteration;
		int max_main_loop_iteration_time = 0;
		SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));

		

		//SDL_SetRelativeMouseMode(SDL_TRUE);

	    while (!quit)
	    {
	    	viewMatrix = createViewMatrix(camera, cameraYaw, cameraPitch);

	    	beforeAtBegginingOfTheIteration = clock();

	    	Vector3 displacement = {0, 0, 0};
	    	Vector3 forward = {-sin(DEGREE_TO_RADIANS(cameraYaw)), 1.0,  cos(DEGREE_TO_RADIANS(cameraYaw))};
	    	//printVector3(&forward);
	    	Vector3 vec;
	    	float x = forward.x * SPEED;
	    	float z = forward.z * SPEED;

	    	// don't use SDL_WaitEvent here or else it would update only if some event is done
	        
	        SDL_Event event;
	        while (SDL_PollEvent(&event))
	        {
		        if(event.type == SDL_QUIT)
		        {
		            quit = 1;
		        }
		        // handle window resize
		        else if(event.type == SDL_WINDOWEVENT)
		        {
		        	if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			            window_width = event.window.data1;
			            window_height = event.window.data2;

			            SDL_SetWindowSize(window, window_width, window_height);

			            SDL_DestroyTexture(texture);

			            texture = SDL_CreateTexture(renderer,
	    SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

			            resizePixelBuffer(window_width, window_height, 0x00000000, &pixelBuffer);
			            createProjectionMatrix(DEGREE_TO_RADIANS(FOV), (float)window_width/(float)window_height, NEAR_PLANE, FAR_PLANE);
			        }
		        }
		        
		        #if (USING_ANDROID_PHONE == 1)

		        else if(event.type == SDL_FINGERDOWN)
		        {
		        	int mouseX;
			    	int mouseY;
			    	SDL_GetMouseState(&mouseX, &mouseY);

			    	if(mouseY > WINDOW_HEIGHT - WINDOW_HEIGHT / 4)
			    	{
			    		vec = Vec3_create(-x, 0, -z);
			        	//printVector3(&vec);
			            displacement = Vectors3_add(&displacement, &vec);
			    	}
			    	else if(mouseY < WINDOW_HEIGHT / 4)
			    	{
			    		vec = Vec3_create(x, 0, z);
				        displacement = Vectors3_add(&displacement, &vec);
			    	}
			    	if(mouseX > WINDOW_HEIGHT - WINDOW_HEIGHT / 4)
			    	{
			    		vec = Vec3_create(z, 0, -x);
				        displacement = Vectors3_add(&displacement, &vec);
			    	}
				        	
				    else if(mouseX < WINDOW_WIDTH/ 4)
				    {
				    	vec = Vec3_create(-z, 0, x);
			            displacement = Vectors3_add(&displacement, &vec);
			            break;
				    }
				        	

		        }
		        #else
		        else if(event.type == SDL_KEYDOWN)
		        {

		        	switch (event.key.keysym.sym)
		        	{
				        case SDLK_UP:
				        	vec = Vec3_create(-x, 0, -z);
				            displacement = Vectors3_add(&displacement, &vec);
				            break;
				        case SDLK_DOWN:
				            vec = Vec3_create(x, 0, z);
				            displacement = Vectors3_add(&displacement, &vec);
				            break;
				        case SDLK_RIGHT:
				        	vec = Vec3_create(z, 0, -x);
				            displacement = Vectors3_add(&displacement, &vec);
				            break;
				        case SDLK_LEFT:
				        	vec = Vec3_create(-z, 0, x);
				            displacement = Vectors3_add(&displacement, &vec);
				            break;

				        case SDLK_SPACE:
				            camera.y += SPEED;
				            break;
				        case SDLK_w:
				        	camera.y -= SPEED;

				            break;
				        
				        case SDLK_ESCAPE:
				        	SDL_SetRelativeMouseMode(SDL_FALSE);
				        	break;
				        case SDLK_KP_4:
				        	cameraYaw -= 3.0f;
				        	break;
				        case SDLK_KP_6:
				        	cameraYaw += 3.0f;
				        	break;
				        case SDLK_KP_2:
				        	cameraPitch += 3.0f;
				        	break;
				        case SDLK_KP_8:
				        	cameraPitch -= 3.0f;
				        	break;
				        default:
				        	break;

	    			}
	    			//printVector3(&displacement);
	    			
		        }
		        #endif
		        
		        

		    }

		    Vector3 newpos = Vectors3_add(&camera, &displacement);

		    bool colliding_with_a_cube = false;
	        for(ulong i = 0 ; i < cubes.length ; i++)
	        {
	        	if(checkCubeCollision(((Entity*)Vector_element(&cubes, i))->position, ((Entity*)Vector_element(&cubes, i))->scale, newpos, Vec3_create(0, 0, 0)))
	        	{
	        		colliding_with_a_cube = true;
	        		break;
	        	}
	        	
	        }

	        if(!colliding_with_a_cube || true)
	        {
	        	camera = newpos;
	        }

        	clearColor(&pixelBuffer, BACKGROUND_COLOR);
    		update(&pixelBuffer, &viewMatrix);
        	SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));



	        
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

			clock_t difference = clock() - beforeAtBegginingOfTheIteration;
		  	int msec = difference * 1000 / CLOCKS_PER_SEC;
		  	//printf("It took %d ms to draw freakin triangles\n", msec);
		  	if(msec > max_main_loop_iteration_time)
		  	{
		  		max_main_loop_iteration_time = msec;
		  	}
	    }

	    
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
	 
	    SDL_DestroyWindow(window);
	    SDL_Quit();

	    printf("Max time elapsed to draw an object : %dms\n", max_draw_time);
	    printf("Max time elapsed for a main loop iteration : %dms\n", max_main_loop_iteration_time);
 	
	#endif

	free(pixelBuffer.buffer);

	Vector_Delete(&flatPlanes);
	Vector_Delete(&cubes);
    return 0;
    
}