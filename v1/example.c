/*
	example.c : An example program to render a transformed rectangle to the console.

	OpenGL isn't used at all here, only matrices and vectors math resulted to this in order to render the figure to the console.

	Headers : sh/*.h, memory/*.h, maths/*.h
	All the code included in these headers and this code was written by me.

	Compile with gcc : gcc example.c -o example -lm
	(-lm is to link the math library in order to use cos and sin functions in maths/matrix_transform.h)
	
	Tested and working on linux, if for some reasons you can't get this to work because your windows
	terminal doesn't support colors (which i don't think is the case but anyways) you should replace : 
	at line ~133 : printf("\033[1;35mA\033[0m"); by just printf("A");

	You can play around with these transformations by changing pos, rotation and scale at line ~87-89
	but know that 3D display isn't implemented yet, so changing pos Z, scale Z or rotation X and Y isn't gonna have any effect on the display.
	Oh and also... Stop listening to shitty (post 2017) pop music if you do.

	- TheCrazzXz / PwnHandler aka POWERFUL HACKER 1337 xSuperHackingPuissant 2023
																			 ____
	                                                                         |
	                                                                         |--- THIS WAS NECESSARY
*/

#define WINDOW_WIDTH 100
#define WINDOW_HEIGHT 24

const float FOV = 70.0;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1000.0;

#define DEGREE_TO_RADIANS(a) (2 * M_PI * a / 360) // convert degrees to radians

#include "sh/PixelBuffer.h" 
#include "sh/Drawer.h"

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
		// AB line
		-0.5, 0.5, // A vertex
		0.5, 0.5, // B vertex

		// CD line
		-0.5, -0.5, // C vertex
		0.5, -0.5, // D vertex

		// AC line
		-0.5, 0.5, // A vertex (again)
		-0.5, -0.5, // C vertex (again)

		// BD line
		0.5, 0.5, // B vertex (again)
		0.5, -0.5 // D vertex (again)

		// It would be cool to add support for indices so we don't have to repeat the vertices.
	};
	Vector4 vertex1_out;	
	Vector4 vertex2_out;	

	Vector3 pos = {0, 0, 0};
	Vector3 rotation = {0, 0, 0};
	Vector3 scale = {1.0, 1.0, 1};

	rotation.z += angle;
	Matrix4 transformationMatrix = createTransformationMatrix(pos, rotation, scale);

	for(int i = 0 ; i < sizeof(vertices) / sizeof(float) / 4 ; i += 1 )
	{
		Vector4 vertex1 = {vertices[4 * i], vertices[4 * i+1], 0, 1};
		Vector4 vertex2 = {vertices[4 * i+2], vertices[4 * i+3], 0, 1};

		vertex1_out = Matrix4_multiplyVector(&transformationMatrix, vertex1);
		vertex2_out = Matrix4_multiplyVector(&transformationMatrix, vertex2);

		float drawn_vertices[] = { vertex1_out.x, vertex1_out.y, vertex2_out.x, vertex2_out.y };

		draw2DVertices(drawn_vertices, sizeof(drawn_vertices) / sizeof(float), pixelBuffer);
	}

}

int main(int argc, char ** argv)
{
	PixelBuffer pixelBuffer = createPixelBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 0x00000000);

	/*for(int j = 0 ; j < WINDOW_HEIGHT ; j++)
	{
		for(int i = 0 ; i < WINDOW_WIDTH ; i++)
		{
			Uint32 *pixel = &(pixelBuffer.buffer[j*pixelBuffer.width + i]);
			//printf("[%d/%d] : %x\n", i, j, *pixel);
		}
	}*/
	float angle = 0;
	while(1)
	{
		clearColor(&pixelBuffer, pixelRGB(0, 0, 0));
	    //printf("Updating...\n");
	    update(&pixelBuffer, angle);
		system("clear");
		angle += 0.1f;
		for(int j = 0 ; j < WINDOW_HEIGHT ; j++)
		{
			for(int i = 0 ; i < WINDOW_WIDTH ; i++)
			{
				Uint32 *pixel = &(pixelBuffer.buffer[j*pixelBuffer.width + i]);
				if(*pixel != 0xff000000)
				{
					printf("\033[1;35mA\033[0m");
				}
				else
				{
					printf(" ");
				}
				
			}
			printf("\n");
		}
	}
	


}