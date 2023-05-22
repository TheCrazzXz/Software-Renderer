#include <stdio.h>
#include <string.h>
#include "vec.h"
#include "matrix.h"
#include "matrix_transform.h"
#include <math.h>

#define DEGREE_TO_RADIANS(a) (2 * M_PI * a / 360) // convert degrees to radians

int main(void)
{
	int running = 1;

	Matrix transformationMatrix = getIdentityMatrix(4);

	char buffer[50];

	printf("Transformation matrix tester : Enter help to list commands\n");

	while(running)
	{
		printf("> ");
		scanf("%s", buffer); // some king (kind) of buffer overflow (hasn't anything to do with the context) ??

		if(strcmp(buffer, "quit") == 0)
		{
			running = 0;
			continue;
		}
		else if(strcmp(buffer, "show") == 0)
		{
			printf("Transformation matrix : \n");
			printMatrix(&transformationMatrix);
		}

		else if(strcmp(buffer, "translate") == 0)
		{
			Vector4 translation;
			printf("Enter coordinates of vector4 to translate the matrix with\n");

			printf("Enter x for the vector : ");
			scanf("%f", &translation.x);

			printf("Enter y for the vector : ");
			scanf("%f", &translation.y);

			printf("Enter z for the vector : ");
			scanf("%f", &translation.z);	

			printf("Enter w for the vector : ");
			scanf("%f", &translation.w);	

			transformationMatrix = Matrix4_translate(&transformationMatrix, translation);
			printf("Done transformation\n");

		}

		else if(strcmp(buffer, "rotate") == 0)
		{
			Vector3 rotation_axis;
			float input_angle;
			float rotation_angle;
			printf("Enter coordinates of vector3 for the axis configuration of the rotation\n");

			printf("Enter 1 to enable X rotation : ");
			scanf("%f", &rotation_axis.x);

			printf("Enter 1 to enable Y rotation : ");
			scanf("%f", &rotation_axis.y);

			printf("Enter 1 to enable Z rotation : ");
			scanf("%f", &rotation_axis.z);	

			printf("Enter angle measure configuration [degree/radian] : ");
			scanf("%s", buffer);

			if(strcmp(buffer, "degree") == 0)
			{
				printf("Enter rotation angle in degrees : ");
			}
			else if(strcmp(buffer, "radian") == 0)
			{
				printf(/* IF YOU FIND THIS, GOOD JOB, WRITTEN 14 of may, 2023, check out this amazing band called Anhilator : https://www.youtube.com/watch?v=pTYHYglem-M&pp=ygUXYW5oaWxhdG9yIGFsaWNlIGluIGhlbGw%3D*/"Enter rotation angle in radians : ");
			}
			else
			{
				printf("Error : Invalid angle measure (has to either be degree or radian)\n");
				continue;
			}

			scanf("%f", &input_angle);

			if(strcmp(buffer, "degree") == 0)
			{
				rotation_angle = DEGREE_TO_RADIANS(input_angle);
			}
			else if(strcmp(buffer, "radian") == 0)
			{
				rotation_angle = input_angle;
			}
			else
			{
				printf("Error : Invalid angle measure (has to either be degree or radian) (how did you get there ?)\n");
				continue;
			}
			

			transformationMatrix = Matrix4_rotate(&transformationMatrix, rotation_axis, rotation_angle);
			printf("Done transformation\n");

		}

		else if(strcmp(buffer, "scale") == 0)
		{
			Vector4 scale;
			printf("Enter coordinates of vector4 to scale the matrix with\n");

			printf("Enter x for the vector : ");
			scanf("%f", &scale.x);

			printf("Enter y for the vector : ");
			scanf("%f", &scale.y);

			printf("Enter z for the vector : ");
			scanf("%f", &scale.z);	

			printf("Enter w for the vector : ");
			scanf("%f", &scale.w);	

			transformationMatrix = Matrix4_scale(&transformationMatrix, scale);
			printf("Done transformation\n");

		}
		else if(strcmp(buffer, "identity") == 0)
		{
			transformationMatrix = getIdentityMatrix(4);
			printf("Identititized\n");
		}

		else if(strcmp(buffer, "help") == 0)
		{
			printf("quit : Leave the program\n");
			printf("show : Print content of the transformation matrix\n");
			printf("translate : Translate the matrix by a vector that will be prompted\n");
			printf("rotate : Rotate the matrix around axis that will be prompted by an angle that will be prompted as well\n");
			printf("scale : Scales the matrix by a vector that will be prompted\n");
			printf("identity : Resets the matrix to 4x4 identity matrix\n");
			printf("help : Show this (maybe?)\n");
		}
	}
}