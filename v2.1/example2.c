#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640

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



Matrix4 createTransformationMatrix(Vector3 translation, Vector3 rotation, Vector3 scale)
{
	Matrix4 transformationMatrix = getIdentityMatrix4();

	transformationMatrix = Matrix4_translate(&transformationMatrix, translation);

	Vector3 zAxis = {0, 0, 1};
	transformationMatrix = Matrix4_rotate(&transformationMatrix, zAxis, DEGREE_TO_RADIANS(rotation.z));
	transformationMatrix = Matrix4_scale(&transformationMatrix, scale);

	//printMatrix4(&transformationMatrix);

	return transformationMatrix;
}

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
	float vertices[] = 
	{
		-0.5, 0.5, 
		0.5, 0.5,

		-0.5, -0.5, 
		0.5, -0.5,

		-0.5, 0.5,
		-0.5, -0.5,

		0.5, 0.5,
		0.5, -0.5
	};
	Vector4 vertex1_out;	
	Vector4 vertex2_out;	

	Vector3 pos = {0, 0, 0};
	Vector3 rotation = {0, 0, angle};
	Vector3 scale = {1.0, 1.0, 1};

	

	for(int j = 0 ; j < 360 ; j++)
	{
		/*
			j     x
			-   = -
			360   255

		*/
		
		rotation.z += j;
		scale.x = 20.0f / (float)j;
		scale.y = 20.0f / (float)j;
		//printf("scale.x : %f\n", scale.x);

		Matrix4 transformationMatrix = createTransformationMatrix(pos, rotation, scale);

		for(int i = 0 ; i < sizeof(vertices) / sizeof(float) / 4 ; i += 1 )
		{
			int current = (int)(255 * j / 360);
			vertices2Color(pixelRGB(0, 0, 0), pixelRGB(0, current / (float)i, current));
			Vector4 vertex1 = {vertices[4 * i], vertices[4 * i+1], 0, 1};
			//printVector4(&vertex1);
			Vector4 vertex2 = {vertices[4 * i+2], vertices[4 * i+3], 0, 1};
			//
			//printVector4(&vertex2);		

			vertex1_out = Matrix4_multiplyVector(&transformationMatrix, vertex1);
			vertex2_out = Matrix4_multiplyVector(&transformationMatrix, vertex2);

			float drawn_vertices[] = { vertex1_out.x, vertex1_out.y, vertex2_out.x, vertex2_out.y };

			draw2DVertices(drawn_vertices, sizeof(drawn_vertices) / sizeof(float), pixelBuffer);
		}
	}
	
}

int main(int argc, char ** argv)
{
	
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
	
	PixelBuffer pixelBuffer = createPixelBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 0x00000000);
	
	/*clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
    printf("Updating...\n");
    update(&pixelBuffer);
	printf("Ended updating\n");
	*/
	vertices2Color(pixelRGB(255, 255, 255), pixelRGB(255, 255, 255));
	float angle = 0;
	clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
    update(&pixelBuffer, angle);
    //renderPixelBufferTerminal(&pixelBuffer, false);

	SDL_UpdateTexture(texture, NULL, pixelBuffer.buffer, pixelBuffer.width * sizeof(Uint32));
    
    while (!quit)
    {
        SDL_WaitEvent(&event);
 		
        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
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
 	
	

    return 0;
    
}