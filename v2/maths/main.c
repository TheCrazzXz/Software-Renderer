#include <stdio.h>
#include "vec.h"
#include "matrix.h"
#include "matrix_transform.h"
#include <math.h>
int main(void)
{
	Matrix transformationMatrix = getIdentityMatrix(4);
	float angle = M_PI / 2;
	Vector3 axis = {0, 0, 1};
	Vector4 translation = {1, 2, 3, 0};
	Vector4 scale = {2, 2, 2, 1};
	transformationMatrix = Matrix4_translate(&transformationMatrix, translation);
	transformationMatrix = Matrix4_rotate(&transformationMatrix, axis, angle);
	transformationMatrix = Matrix4_scale(&transformationMatrix, scale);
	printMatrix(&transformationMatrix);

	Vector4 vector = {1, 3, 11, 1};
	Matrix vectorMatrix = vector4toMatrix(vector);
	printMatrix(&vectorMatrix);

	Matrix transformedVector = Matrix_multiply(&transformationMatrix, &vectorMatrix);
	printMatrix(&transformedVector);


}