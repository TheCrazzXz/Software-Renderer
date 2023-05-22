#define WINDOW_WIDTH_CONSOLE 150
#define WINDOW_HEIGHT_CONSOLE 42

#define WINDOW_WIDTH_WINDOW 800
#define WINDOW_HEIGHT_WINDOW 640

const float FOV = 70.0;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1000.0;

#define SPEED 0.2
#define MAX_RENDERING_DISTANCE 50

#define DEGREE_TO_RADIANS(a) (2 * M_PI * a / 360) // convert degrees to radians

#include "sh/PixelBuffer.h" 
#include "sh/Drawer.h"
#include "sh/TerminalRenderer.h"

#include "maths/matrix.h"
#include "maths/matrix4.h"
#include "maths/matrix_transform.h"

#include "maths/projection.h"
#include "maths/maths.h"

#include "shapes.h"

/*
	WINDOW_ENABLE = 0 : Don't use SDL, don't include it and don't use any window, render on console
	WINDOW_ENABLE = 1 : Import SDL from <SDL2/SDL.h> and use a window to render	

	USING_ANDROID_PHONE : If using an android phone, use input from it 

*/

#define USING_ANDROID_PHONE 0

#define ANDROID_PHONE_SCREEN_WIDTH 1400 
#define ANDROID_PHONE_SCREEN_HEIGHT 1400 


#define WINDOW_ENABLE 1

#if WINDOW_ENABLE == 0

#define WINDOW_WIDTH WINDOW_WIDTH_CONSOLE
#define WINDOW_HEIGHT WINDOW_HEIGHT_CONSOLE

#else

#if USING_ANDROID_PHONE

#define WINDOW_WIDTH ANDROID_PHONE_SCREEN_WIDTH
#define WINDOW_HEIGHT ANDROID_PHONE_SCREEN_HEIGHT

#else

#define WINDOW_WIDTH WINDOW_WIDTH_WINDOW
#define WINDOW_HEIGHT WINDOW_HEIGHT_WINDOW

#endif
#include <SDL2/SDL.h>

#endif



int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

#define BACKGROUND_COLOR pixelRGB(0, 255, 255)

Vector3 camera = {0.0, 0.5, 20};
float cameraPitch = 0.0f;
float cameraYaw = 0.0f;

const ulong square_drawing_scale_from_base_x = 10;
const ulong square_drawing_scale_from_base_y = 10;

clock_t beforeAtSquareDraw;
int max_square_draw_time = 0;

void update(PixelBuffer* pixelBuffer, float angle)
{
	float z = 0;
	Vector3 pos = {0, 3, 20};
	Vector3 cubes_pos = {0, 5, 0};
	Vector3 rotation = {0, 0, 0};
	Vector3 cubes_rotation = {0, 0, 0};
	Vector3 cubes_scale = {4, 5, 4};

	
	Vector3 flatPlanePos = {0, 0, 0};
	Vector3 flatPlaneRotation = {0, 0, 0};
	Vector3 flatPlaneScale = {2, 0, 2};

	Matrix4 viewMatrix = createViewMatrix(camera, cameraYaw, cameraPitch);

	//transformationMatrix = createTransformationMatrix(pos, rotation, cubes_scale);
		
	for(long x = -500 ; x < 504 ; x += 4)
	{
		for(long z = -504 ; z < 504 ; z += 4)
		{
			flatPlanePos.x = x;
			flatPlanePos.z = z;
			if(Vectors3_distance(&camera, &flatPlanePos) <= MAX_RENDERING_DISTANCE)
			{
				flatPlane(flatPlanePos, flatPlaneRotation, flatPlaneScale, &viewMatrix, pixelBuffer, pixelRGB(0, 0, 0));
			}
			
		}
	} 
	//flatPlane(flatPlanePos, flatPlaneRotation, flatPlaneScale, &viewMatrix, pixelBuffer, pixelRGB(0, 0, 0));
	
	cubes_pos.z = 0;
	cubes_rotation.y = 0;
	for(ulong i = 0 ; i < 3 ; i++)
	{
		cubes_pos.x -= 2.0f * cubes_scale.x;
		//cubes_rotation.x += 15.0f;
		if(Vectors3_distance(&camera, &cubes_pos) <= MAX_RENDERING_DISTANCE)
		{
			cube(cubes_pos, cubes_rotation, cubes_scale, &viewMatrix, pixelBuffer, pixelRGB(255, 255, 255));
		}
	}
	rotation.y = 0;
	

	printf("Camera : \n");
	printVector3(&camera);

	beforeAtSquareDraw = clock(); 

	//printf("square iterations for drawing = %lu * %lu = %lu\n", square_drawing_scale_from_base_x, square_drawing_scale_from_base_y, square_drawing_scale_from_base_x * square_drawing_scale_from_base_y);
	for(int x = 0 ; x < 800 ; x++)
	{
		for(int y = 0 ; y < 640 ; y++)
		{
			//pixelAtPosition(x, y, 0xffffffff, pixelBuffer);
		}
	}
	clock_t difference = clock() - beforeAtSquareDraw;
  	int msec = difference * 1000 / CLOCKS_PER_SEC;
  	//printf("It took %d ms to draw freakin triangles\n", msec);
  	if(msec > max_square_draw_time)
  	{
  		max_square_draw_time = msec;
  	}


}

int main(int argc, char ** argv)
{
	createProjectionMatrix(DEGREE_TO_RADIANS(FOV), (float)window_width/(float)window_height, NEAR_PLANE, FAR_PLANE);
	PixelBuffer pixelBuffer = createPixelBuffer(window_width, window_height, BACKGROUND_COLOR);

	float angle = 45;

	clearColor(&pixelBuffer, BACKGROUND_COLOR);
	update(&pixelBuffer, angle);

	#if (WINDOW_ENABLE == 0)
	while(1)
	{
		
		angle += 1.0f;
		clearColor(&pixelBuffer, BACKGROUND_COLOR);
	    update(&pixelBuffer, angle);
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
	    	



	    	beforeAtBegginingOfTheIteration = clock();

	    	angle++;
	    	Vector3 displacement = {0, 0, 0};
	    	Vector3 forward = {-sin(DEGREE_TO_RADIANS(cameraYaw)), 1.0,  cos(DEGREE_TO_RADIANS(cameraYaw))};
	    	printVector3(&forward);
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

			    	if(mouseY > WINDOW_HEIGHT / 2)
			    	{
			    		vec = Vec3_create(-x, 0, -z);
			        	//printVector3(&vec);
			            displacement = Vectors3_add(&displacement, &vec);
			    	}
			    	else
			    	{
			    		vec = Vec3_create(x, 0, z);
				        displacement = Vectors3_add(&displacement, &vec);
			    	}

		        }
		        #else
		        else if(event.type == SDL_KEYDOWN)
		        {

		        	switch (event.key.keysym.sym)
		        	{
				        case SDLK_UP:
				        	vec = Vec3_create(-x, 0, -z);
				        	printVector3(&vec);
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
				        case SDLK_LSHIFT:
				            camera.y -= SPEED;
				            break;
				        
				        case SDLK_ESCAPE:
				        	SDL_SetRelativeMouseMode(SDL_FALSE);
				        	break;
				        case SDLK_KP_4:
				        	cameraYaw -= 10.0f;
				        	break;
				        case SDLK_KP_6:
				        	cameraYaw += 10.0f;
				        	break;
				        case SDLK_KP_2:
				        	cameraPitch -= 1.0f;
				        	break;
				        case SDLK_KP_8:
				        	cameraPitch += 1.0f;
				        	break;
				        default:
				        	break;

	    			}
	    			//printVector3(&displacement);
	    			
		        }
		        #endif
		        camera = Vectors3_add(&camera, &displacement);

		    }

        	clearColor(&pixelBuffer, BACKGROUND_COLOR);
    		update(&pixelBuffer, angle);
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
	    printf("Max square draw time  : %dms\n", max_square_draw_time);
 	
	#endif

	free(pixelBuffer.buffer);

    return 0;
    
}