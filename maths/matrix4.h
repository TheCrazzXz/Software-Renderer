#pragma once

#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long ulong;

#define MATRIX_ELEMENT(matrix, col, line) (matrix->data[line * (matrix->size[0]) + col])
#define MATRIX_ELEMENT_VALUE(matrix, col, line) (matrix.data[line * (matrix.size[0]) + col])

#define m0(mat) (MATRIX_ELEMENT_VALUE(mat, 0, 0))
#define m1(mat) (MATRIX_ELEMENT_VALUE(mat, 1, 0))
#define m2(mat) (MATRIX_ELEMENT_VALUE(mat, 2, 0))
#define m3(mat) (MATRIX_ELEMENT_VALUE(mat, 3, 0))

#define m4(mat) (MATRIX_ELEMENT_VALUE(mat, 0, 1))
#define m5(mat) (MATRIX_ELEMENT_VALUE(mat, 1, 1))
#define m6(mat) (MATRIX_ELEMENT_VALUE(mat, 2, 1))
#define m7(mat) (MATRIX_ELEMENT_VALUE(mat, 3, 1))

#define m8(mat) (MATRIX_ELEMENT_VALUE(mat, 0, 2))
#define m9(mat) (MATRIX_ELEMENT_VALUE(mat, 1, 2))
#define m10(mat) (MATRIX_ELEMENT_VALUE(mat, 2, 2))
#define m11(mat) (MATRIX_ELEMENT_VALUE(mat, 3, 2))

#define m12(mat) (MATRIX_ELEMENT_VALUE(mat, 0, 3))
#define m13(mat) (MATRIX_ELEMENT_VALUE(mat, 1, 3))
#define m14(mat) (MATRIX_ELEMENT_VALUE(mat, 2, 3))
#define m15(mat) (MATRIX_ELEMENT_VALUE(mat, 3, 3))


#define m0p(mat) (MATRIX_ELEMENT(mat, 0, 0))
#define m1p(mat) (MATRIX_ELEMENT(mat, 1, 0))
#define m2p(mat) (MATRIX_ELEMENT(mat, 2, 0))
#define m3p(mat) (MATRIX_ELEMENT(mat, 3, 0))

#define m4p(mat) (MATRIX_ELEMENT(mat, 0, 1))
#define m5p(mat) (MATRIX_ELEMENT(mat, 1, 1))
#define m6p(mat) (MATRIX_ELEMENT(mat, 2, 1))
#define m7p(mat) (MATRIX_ELEMENT(mat, 3, 1))

#define m8p(mat) (MATRIX_ELEMENT(mat, 0, 2))
#define m9p(mat) (MATRIX_ELEMENT(mat, 1, 2))
#define m10p(mat) (MATRIX_ELEMENT(mat, 2, 2))
#define m11p(mat) (MATRIX_ELEMENT(mat, 3, 2))

#define m12p(mat) (MATRIX_ELEMENT(mat, 0, 3))
#define m13p(mat) (MATRIX_ELEMENT(mat, 1, 3))
#define m14p(mat) (MATRIX_ELEMENT(mat, 2, 3))
#define m15p(mat) (MATRIX_ELEMENT(mat, 3, 3))


#include "vec.h"

struct Matrix4
{
	float data[4 * 4];
	ulong size[2]; // size[0] : nCols ; size[1] : nLines
};

/*
	Matrix4_create : Returns a 4x4 matrix

	Notice : This function doesn't clear the matrix, you may find values that aren't null among all elements

*/
struct Matrix4 Matrix4_create()
{
	struct Matrix4 matrix;
	matrix.size[0] = 4;
	matrix.size[1] = 4;
	return matrix;
}

void matrix4Error(struct Matrix* matrix4, const char* message)
{
	printf("Error with matrix %p : %s", matrix4, message);
	exit(-1);
}

/*
	Matrix4_element : Returns a pointer to the element at certain column and certain line

*/
float* Matrix4_element(struct Matrix4* matrix, ulong col, ulong line)
{
	if(col > matrix->size[0] || line > matrix->size[1])
	{
		printf("Error with matrix4 : Index out of bounds");
		exit(-1);
	}
	float* data_ptr;
	data_ptr = (matrix->data);
	if(line*matrix->size[0] + col <= matrix->size[0] * matrix->size[1])
	{
		return &data_ptr[line*matrix->size[0] + col];
	}
	printf("Error with matrix %p : Index {%lu ; %lu} out of bounds for size %lu*%lu (%lu/%lu)\n", matrix, col, line, matrix->size[0], matrix->size[1], line*matrix->size[0] + col, matrix->size[0] * matrix->size[1]);
	exit(-1);
	
}

/*
	Matrix_clear : Sets all elements of a matrix to 0's

*/

void Matrix4_clear(struct Matrix4* matrix)
{
	for(ulong j = 0 ; j < matrix->size[1] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[0] ; i++)
		{
			MATRIX_ELEMENT(matrix, i, j) = 0.0;
		}
	}
}
/*
	matrixCopy : creates a new allocated matrix that has same number of columns, same number of lines and same elements as another matrix

*/
struct Matrix4 matrix4Copy(struct Matrix4* matSrc)
{
	struct Matrix4 newMatrix;
	
	newMatrix = Matrix4_create();
	for(ulong j = 0 ; j < newMatrix.size[0] ; j++)
	{
		for(ulong i = 0 ; i < newMatrix.size[1] ; i++)
		{
			MATRIX_ELEMENT_VALUE(newMatrix, i, j) = MATRIX_ELEMENT(matSrc, i, j);
		}
	}
	return newMatrix;
}
/*
	Matrix_identity : Sets the identity matrix to a matrix

*/
void Matrix4_identity(struct Matrix4* matrix)
{
	for(ulong j = 0 ; j < matrix->size[0] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[1] ; i++)
		{
			if(i == j)
			{
				MATRIX_ELEMENT(matrix, i, j) = 1.0;
			}
			else
			{
				MATRIX_ELEMENT(matrix, i, j) = 0.0;
			}
			
		}
	}
}

/*
	getIdentityMatrix : Returns a new allocated matrix which is the identity matrix

*/
struct Matrix4 getIdentityMatrix4()
{
	struct Matrix4 matrix = Matrix4_create();

	for(ulong j = 0 ; j < matrix.size[0] ; j++)
	{
		for(ulong i = 0 ; i < matrix.size[1] ; i++)
		{
			if(i == j)
			{
				MATRIX_ELEMENT_VALUE(matrix, i, j) = 1.0;
			}
			else
			{
				MATRIX_ELEMENT_VALUE(matrix, i, j) = 0.0;
			}
			
		}
	}
	return matrix;
}

/*
	printMatrix : Prints the size and elements of a matrix to STDOUT

*/

void printMatrix4(struct Matrix4* matrix);

/*
	Matrices_add : Adds two matrices

*/

struct Matrix4 Matrices4_add(struct Matrix4* matA, struct Matrix4* matB)
{
	struct Matrix4 matrix;
	matrix = matrix4Copy(matA);

	for(ulong j = 0 ; j < matA->size[0] ; j++)
	{
		for(ulong i = 0 ; i < matA->size[1] ; i++)
		{
			MATRIX_ELEMENT_VALUE(matrix, i, j) = MATRIX_ELEMENT(matA, i, j) + MATRIX_ELEMENT(matB, i, j);
		}
	}
	return matrix;
}

/*
	Matrix_multiplyNumber : Multiply a matrix by a real number

*/

struct Matrix4 Matrix4_multiplyNumber(struct Matrix4* mat, float num)
{
	struct Matrix4 matrix;
	matrix = matrix4Copy(mat);

	for(ulong j = 0 ; j < mat->size[0] ; j++)
	{
		for(ulong i = 0 ; i < mat->size[1] ; i++)
		{
			MATRIX_ELEMENT_VALUE(matrix, i, j) = num * (MATRIX_ELEMENT(mat, i, j));
		}
	}
	return matrix;
}

/*
	Matrix_multiply : Mutliplies a matrix with another matrix if the number of columns of the first matrix is strictly equal to the number of lines of the second matrix

*/

struct Matrix4 Matrix4_multiply(struct Matrix4* matA, struct Matrix4* matB)
{
	if(matA->size[0] != matB->size[1])
	{
		printf("Error while trying to multiply %p with %p : Number of colums of first matrix (%lu) is different than the number of columns of the other matrix (%lu)\n", matA, matB, matA->size[0], matB->size[1]); 
		exit(-1);
	}
	//printf("MatA size[0] = %lu\n", matB->size[0]);
	//printf("MatB size[1] = %lu\n", matA->size[1]);
	struct Matrix4 newMatrix = {
		{
m0p(matA) * m0p(matB) + m1p(matA) * m4p(matB) + m2p(matA) * m8p(matB) + m3p(matA) * m12p(matB),

m0p(matA) * m1p(matB) + m1p(matA) * m5p(matB) + m2p(matA) * m9p(matB) + m3p(matA) * m13p(matB),

m0p(matA) * m2p(matB) + m1p(matA) * m6p(matB) + m2p(matA) * m10p(matB) + m3p(matA) * m14p(matB),

m0p(matA) * m3p(matB) + m1p(matA) * m7p(matB) + m2p(matA) * m11p(matB) + m3p(matA) * m15p(matB),



m4p(matA) * m0p(matB) + m5p(matA) * m4p(matB) + m6p(matA) * m8p(matB) + m7p(matA) * m12p(matB),

m4p(matA) * m1p(matB) + m5p(matA) * m5p(matB) + m6p(matA) * m9p(matB) + m7p(matA) * m13p(matB),

m4p(matA) * m2p(matB) + m5p(matA) * m6p(matB) + m6p(matA) * m10p(matB) + m7p(matA) * m14p(matB),

m4p(matA) * m3p(matB) + m5p(matA) * m7p(matB) + m6p(matA) * m11p(matB) + m7p(matA) * m15p(matB),



m8p(matA) * m0p(matB) + m9p(matA) * m4p(matB) + m10p(matA) * m8p(matB) + m11p(matA) * m12p(matB),

m8p(matA) * m1p(matB) + m9p(matA) * m5p(matB) + m10p(matA) * m9p(matB) + m11p(matA) * m13p(matB),

m8p(matA) * m2p(matB) + m9p(matA) * m6p(matB) + m10p(matA) * m10p(matB) + m11p(matA) * m14p(matB),

m8p(matA) * m3p(matB) + m9p(matA) * m7p(matB) + m10p(matA) * m11p(matB) + m11p(matA) * m15p(matB),



m12p(matA) * m0p(matB) + m13p(matA) * m4p(matB) + m14p(matA) * m8p(matB) + m15p(matA) * m12p(matB),

m12p(matA) * m1p(matB) + m13p(matA) * m5p(matB) + m14p(matA) * m9p(matB) + m15p(matA) * m13p(matB),

m12p(matA) * m2p(matB) + m13p(matA) * m6p(matB) + m14p(matA) * m10p(matB) + m15p(matA) * m14p(matB),

m12p(matA) * m3p(matB) + m13p(matA) * m7p(matB) + m14p(matA) * m11p(matB) + m15p(matA) * m15p(matB)
},
		{4, 4}
	};

	return newMatrix;
}

/*
	Matrix4_multiplyVector : Returns matrix * vector, as a Vector4

*/

struct Vec4 Matrix4_multiplyVector(struct Matrix4* mat, struct Vec4 vec)
{
	struct Vec4 result;

	result.x = (MATRIX_ELEMENT(mat, 0, 0)) * vec.x 
									  + (MATRIX_ELEMENT(mat, 1, 0)) * vec.y 
									  + (MATRIX_ELEMENT(mat, 2, 0)) * vec.z 
									  + (MATRIX_ELEMENT(mat, 3, 0)) * vec.w;

	result.y = (MATRIX_ELEMENT(mat, 0, 1)) * vec.x 
									  + (MATRIX_ELEMENT(mat, 1, 1)) * vec.y 
									  + (MATRIX_ELEMENT(mat, 2, 1)) * vec.z 
									  + (MATRIX_ELEMENT(mat, 3, 1)) * vec.w;

	result.z = (MATRIX_ELEMENT(mat, 0, 2)) * vec.x 
									  + (MATRIX_ELEMENT(mat, 1, 2)) * vec.y 
									  + (MATRIX_ELEMENT(mat, 2, 2)) * vec.z 
									  + (MATRIX_ELEMENT(mat, 3, 2)) * vec.w;

	result.w = (MATRIX_ELEMENT(mat, 0, 3)) * vec.x 
									  + (MATRIX_ELEMENT(mat, 1, 3)) * vec.y 
									  + (MATRIX_ELEMENT(mat, 2, 3)) * vec.z 
									  + (MATRIX_ELEMENT(mat, 3, 3)) * vec.w;

	return result;
}

void printMatrix4(struct Matrix4* matrix)
{
	printf("Matrix4 (%lux%lu)\n", matrix->size[0], matrix->size[1]);
	for(ulong j = 0 ; j < matrix->size[1] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[0] ; i++)
		{
			printf("%f ", MATRIX_ELEMENT(matrix, i, j));
		}
		printf("\n");
	}
}


typedef struct Matrix4 Matrix4;

#endif