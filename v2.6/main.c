/*
	V2.6 : A lot of updates and optimization

	- Finished with clipping problems
	- Now really supports triangle filling, optimized by using memset to draw horizontal lines
	- Improved interpolation (this example doesn't use it)

	You can now draw objects anywhere on the space and fill them, it should work

	Use bash ./build.sh to compile or you can use your compiler if you aren't on linux
	TheCrazzXz, 4th of June, 2023

	S
	|	L
	|	   	A
	|    /		Y
	|  /		    E
	|___________________R

*/

#include "graphical_library/graphics/DataDrawing.h"
#include "graphical_library/graphics/shapeDrawing.h"
#include "graphical_library/maths/vec.h"
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
#include "engine/windowInput.h"

#endif

const float FOV = 120.0;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1000.0;

#define SPEED 2
#define MOUSE_SENSIBILITY 0.75



#include "graphical_library/graphics/PixelBuffer.h" 
#include "graphical_library/graphics/TerminalRenderer.h"

#include "graphical_library/maths/matrix.h"
#include "graphical_library/maths/matrix4.h"
#include "graphical_library/maths/matrix_transform.h"

#include "graphical_library/maths/projection.h"
#include "graphical_library/maths/maths.h"

#include "engine/entity.h"

#include <time.h>
#include "engine/shapes.h"
#include "engine/Camera.h"

#define RENDER_ONLY_ONCE false

int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

#define BACKGROUND_COLOR pixelRGB(0, 0, 0)

Camera camera = { {0, 0, 10}, 0,0.0f, SPEED, MOUSE_SENSIBILITY};

Vector flatPlanes;
Vector cubes;
Vector floorVec;

float color = 0;

#define DO_THE_FUN_TRANSFORMATIONS true

void update(PixelBuffer* pixelBuffer, Matrix4* viewMatrix)
{
	for(ulong i = 0 ; i < flatPlanes.length ; i++ )
	{
		if(DO_THE_FUN_TRANSFORMATIONS)
		{
			((Entity*)Vector_element(&flatPlanes, i))->rotation.x++;
			((Entity*)Vector_element(&flatPlanes, i))->rotation.y--;
		}
		
		
		Entity_render(Vector_element(&flatPlanes, i), viewMatrix, pixelBuffer, &camera.position);
	}
	for(ulong i = 0 ; i < cubes.length ; i++ )
	{
		if(DO_THE_FUN_TRANSFORMATIONS)
		{
			((Entity*)Vector_element(&cubes, i))->rotation.x--;
			((Entity*)Vector_element(&cubes, i))->rotation.z--;
			((Entity*)Vector_element(&cubes, i))->color_0 = Vec4_create(1.0f - color + (float)i / (float)cubes.length, color / 2 + 1 - (float)i / (float)cubes.length, color, 1);
		}
		Entity_render(Vector_element(&cubes, i), viewMatrix, pixelBuffer, &camera.position);
	}
	for(ulong i = 0 ; i < floorVec.length ; i++)
	{
		Entity_render(Vector_element(&floorVec, i), viewMatrix, pixelBuffer, &camera.position);
	}

	color += 0.01f;
	if(color >= 1)
	{
		color = 0;
	}
}

int main(int argc, char ** argv)
{
	// Fill and wireframe
	//shSetDrawLevel(DRAWER_DRAW_LEVEL_FILL_AND_WIREFRAME);

	// Only fill shapes :
	shSetDrawLevel(DRAWER_DRAW_LEVEL_FILL);

	// Only wireframe shapes (ony shape borders)
	//shSetDrawLevel(DRAWER_DRAW_LEVEL_WIREFRAME);

	// Only draw vertices as points
	//shSetDrawLevel(DRAWER_DRAW_LEVEL_POINTS);

	Matrix4 viewMatrix = createViewMatrix(camera.position, camera.cameraYaw, camera.cameraPitch);
	flatPlanes = Vector_create(sizeof(Entity));
	cubes = Vector_create(sizeof(Entity));

	floorVec = Vector_create(sizeof(Entity));

	createProjectionMatrix(DEGREE_TO_RADIANS(FOV), (float)window_width/(float)window_height, NEAR_PLANE, FAR_PLANE);
	PixelBuffer pixelBuffer = createPixelBuffer(window_width, window_height, BACKGROUND_COLOR);

	Vector3 position = {0, 0.0, 0.0};
	Vector3 rotation = {0, 0, 0};
	Vector3 scale = {1, 1, 1};

	const float destination = 30.0f;
	
	for(float x = 0 ; x < destination ; x += 4 )
	{
		Entity cube = createEntity(cubeModel, Vec3_create(x, x, x + 0), rotation, scale, Vec4_create(1, 1, 1, 1));
		Vector_Push_back(&cubes, &cube);
	}

	for(float x = 0 ; x < destination ; x += 4 )
	{
		Entity cube = createEntity(cubeModel, Vec3_create(x, x, x + 6), rotation, scale, Vec4_create(1, 1, 1, 1));
		Vector_Push_back(&cubes, &cube);
	}

	for(float x = 0 ; x < destination ; x += 4 )
	{
		Entity flatPlane = createEntity(cubeModel, Vec3_create(x, x, x + 4), rotation, scale, Vec4_create(x / destination, 1 - x / destination, 0, 1));
		
		Vector_Push_back(&flatPlanes, &flatPlane);
	}

	for(float x = 0 ; x < destination ; x += 4 )
	{
		Entity flatPlane = createEntity(cubeModel, Vec3_create(x, x, x + 2), rotation, scale, Vec4_create(x / destination, 1, 1 - x / destination, 1));
		
		Vector_Push_back(&flatPlanes, &flatPlane);
	}

	/*for(float x = 0 ; x < destination ; x += 2 )
	{
		for(float z = 0 ; z < destination ; z += 2)
		{
			Entity flatPlane = createEntity(flatPlaneModel, Vec3_create(x, 0, z), rotation, scale, Vec4_create(1, 1, 1, 1));
		
			Vector_Push_back(&floorVec, &flatPlane);
		}
		
	}*/


	//mapToCubes(&cubes, worldMap, 16, 16, 2);

	//square = createEntity(square_2d_Model, position, rotation, scale, 0xffff0000, 0xff00ff00, 0xff0000ff);

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
	    	camera.position.z -= SPEED;
	    }
	    else if(buffer == 's')
	    {
	    	camera.position.z += SPEED;
	    }
	    else if(buffer == 'q')
	    {
	    	camera.position.x -= SPEED;
	    }
	    else if(buffer == 'd')
	    {
	    	camera.position.x += SPEED;
	    }

	    system("clear");
	}
	#endif
	

    #if (WINDOW_ENABLE == 1)
	    int quit = 0;
	    
	    
	    SDL_Event event;
	 
	    SDL_Init(SDL_INIT_VIDEO);
	 
	    SDL_Window * window = SDL_CreateWindow("Graphics",
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
		
		bool rendered_first_frame = false;

	    while (!quit)
	    {
	    	Vector3 displacement = {0, 0, 0};
	    	viewMatrix = createViewMatrix(camera.position, camera.cameraYaw, camera.cameraPitch);

	    	beforeAtBegginingOfTheIteration = clock();

	    	

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
		        else if( event.type == SDL_MOUSEBUTTONDOWN )
			    {
			        //If the left mouse button was pressed
			        if( event.button.button == SDL_BUTTON_LEFT )
			        {
			        	SDL_SetRelativeMouseMode(SDL_TRUE);
			        }
			    }
    

		        else if(event.type == SDL_MOUSEMOTION)
		        {
		        	if(SDL_GetRelativeMouseMode() == SDL_TRUE)
		        	{
		        		windowMouseInput(event, &camera);
		        	}
		        	
		        	
		        }
		        else if(event.type == SDL_KEYDOWN)
		        {
		        	windowKeyboardCameraInput(event, &camera, &displacement);
		        }
		        	
		        	
	    			//printVector3(&displacement);
	    			
		        #endif
		    }

		    Vector3 newpos = Vectors3_add(&camera.position, &displacement);

		    bool colliding_with_a_cube = false;

	        if(!colliding_with_a_cube)
	        {
	        	camera.position = newpos;
	        }
	        if((RENDER_ONLY_ONCE && !rendered_first_frame) || !RENDER_ONLY_ONCE)
	        {
	        	clearColor(&pixelBuffer, BACKGROUND_COLOR);

	    		update(&pixelBuffer, &viewMatrix);
	        	SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));
	        	rendered_first_frame = true;
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

	    printf("Max time elapsed for a main loop iteration : %dms\n", max_main_loop_iteration_time);
 	
	#endif

	free(pixelBuffer.buffer);

	Vector_Delete(&flatPlanes);
	Vector_Delete(&cubes);
    return 0;
    
}