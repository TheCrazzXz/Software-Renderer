#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long ulong;

#include "vec.h"

struct Matrix
{
	float* data;
	ulong size[2]; // size[0] : nCols ; size[1] : nLines
};

void matrixError(struct Matrix* matrix, const char* message)
{
	printf("Error with matrix %p : %s", matrix, message);
	exit(-1);
}
/*
	Matrix_create : Returns a new allocated matrix of xSize number of columns and ySize number of lines

	Notice : This function doesn't clear the matrix, you may find values that aren't null among all elements

*/
struct Matrix Matrix_create(ulong xSize, ulong ySize)
{
	struct Matrix matrix;
	matrix.size[0] = xSize;
	matrix.size[1] = ySize;
	matrix.data = malloc(xSize * ySize * sizeof(float));
	if(!matrix.data)
	{
		perror("malloc");
	}
	return matrix;
}
/*
	Matrix_element : Returns a pointer to the element at certain column and certain line

*/
float* Matrix_element(struct Matrix* matrix, ulong col, ulong line)
{
	if(col > matrix->size[0] || line > matrix->size[1])
	{
		matrixError(matrix, "Index out of bound for matrix\n");
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

void Matrix_clear(struct Matrix* matrix)
{
	for(ulong j = 0 ; j < matrix->size[1] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[0] ; i++)
		{
			*Matrix_element(matrix, i, j) = 0.0;
		}
	}
}
/*
	matrixCopy : creates a new allocated matrix that has same number of columns, same number of lines and same elements as another matrix

*/
struct Matrix matrixCopy(struct Matrix* matSrc)
{
	struct Matrix newMatrix;
	newMatrix = Matrix_create(matSrc->size[1], matSrc->size[0]);
	for(ulong j = 0 ; j < newMatrix.size[0] ; j++)
	{
		for(ulong i = 0 ; i < newMatrix.size[1] ; i++)
		{
			*Matrix_element(&newMatrix, i, j) = *Matrix_element(matSrc, i, j);
		}
	}
	return newMatrix;
}
/*
	Matrix_identity : Sets the identity matrix to a matrix

*/
void Matrix_identity(struct Matrix* matrix)
{
	for(ulong j = 0 ; j < matrix->size[0] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[1] ; i++)
		{
			if(i == j)
			{
				*Matrix_element(matrix, i, j) = 1.0;
			}
			else
			{
				*Matrix_element(matrix, i, j) = 0.0;
			}
			
		}
	}
}
/*
	getIdentityMatrix : Returns a new allocated matrix which is the identity matrix

*/
struct Matrix getIdentityMatrix(float size)
{
	struct Matrix matrix = Matrix_create(size, size);

	for(ulong j = 0 ; j < matrix.size[0] ; j++)
	{
		for(ulong i = 0 ; i < matrix.size[1] ; i++)
		{
			if(i == j)
			{
				*Matrix_element(&matrix, i, j) = 1.0;
			}
			else
			{
				*Matrix_element(&matrix, i, j) = 0.0;
			}
			
		}
	}
	return matrix;
}
/*
	Matrix_delete : Frees a matrix memory

*/
void Matrix_delete(struct Matrix* matrix)
{
	free(matrix->data);
}
/*
	printMatrix : Prints the size and elements of a matrix to STDOUT

*/

void printMatrix(struct Matrix* matrix);

/*
	Matrices_add : Adds two matrices

*/

struct Matrix Matrices_add(struct Matrix* matA, struct Matrix* matB)
{
	struct Matrix matrix;
	matrix = matrixCopy(matA);

	for(ulong j = 0 ; j < matA->size[0] ; j++)
	{
		for(ulong i = 0 ; i < matA->size[1] ; i++)
		{
			*Matrix_element(&matrix, i, j) = *Matrix_element(matA, i, j) + *Matrix_element(matB, i, j);
		}
	}
	return matrix;
}

/*
	Matrix_multiplyNumber : Multiply a matrix by a real number

*/

struct Matrix Matrix_multiplyNumber(struct Matrix* mat, float num)
{
	struct Matrix matrix;
	matrix = matrixCopy(mat);

	for(ulong j = 0 ; j < mat->size[0] ; j++)
	{
		for(ulong i = 0 ; i < mat->size[1] ; i++)
		{
			*Matrix_element(&matrix, i, j) = num * (*Matrix_element(mat, i, j));
		}
	}
	return matrix;
}

/*
	Matrix_multiply : Mutliplies a matrix with another matrix if the number of columns of the first matrix is strictly equal to the number of lines of the second matrix

*/

struct Matrix Matrix_multiply(struct Matrix* matA, struct Matrix* matB)
{
	if(matA->size[0] != matB->size[1])
	{
		printf("Error while trying to multiply %p with %p : Number of colums of first matrix (%lu) is different than the number of columns of the other matrix (%lu)\n", matA, matB, matA->size[0], matB->size[1]); 
		exit(-1);
	}
	//printf("MatA size[0] = %lu\n", matB->size[0]);
	//printf("MatB size[1] = %lu\n", matA->size[1]);
	struct Matrix newMatrix = Matrix_create(matB->size[0], matA->size[1]);
	Matrix_clear(&newMatrix);

	// For each line of matA
	for(ulong aLine = 0 ; aLine < matA->size[1] ; aLine++)
	{
		//printf("matA Line : %lu/%lu\n", aLine, matA->size[1]);
		// For each column of matB
		for(ulong bCol = 0 ; bCol < matB->size[0] ; bCol++)
		{
			//printf("\tmatB column : %lu/%lu\n", bCol, matB->size[0]);
			*Matrix_element(&newMatrix, bCol, aLine) = 0;
			for(ulong aCol = 0 ; aCol < matA->size[0] ; aCol++)
			{
				*Matrix_element(&newMatrix, bCol, aLine) += *Matrix_element(matA, aCol, aLine) * (*Matrix_element(matB, bCol, aCol));
				//printf("\t\t Element %lu = %f\n", aCol, *Matrix_element(&newMatrix, bCol, aLine));
			}
			//printf("\t\tWritten at { %lu ; %lu }\n", bCol, aLine);
		}
	}
	return newMatrix;
}

void printMatrix(struct Matrix* matrix)
{
	printf("Matrix (%lux%lu)\n", matrix->size[0], matrix->size[1]);
	for(ulong j = 0 ; j < matrix->size[1] ; j++)
	{
		for(ulong i = 0 ; i < matrix->size[0] ; i++)
		{
			printf("%f ", *Matrix_element(matrix, i, j));
		}
		printf("\n");
	}
}

/*
	vector2toMatrix : Converts a 2D vector to its corresponding 1*2 matrix

*/

struct Matrix vector2toMatrix(struct Vec2 vector)
{
	struct Matrix vectorMatrix = Matrix_create(1, 2);
	*Matrix_element(&vectorMatrix, 0, 0) = vector.x;
	*Matrix_element(&vectorMatrix, 0, 1) = vector.y;
	return vectorMatrix;
}

/*
	vector3toMatrix : Converts a 3D vector to its corresponding 1*3 matrix

*/

struct Matrix vector3toMatrix(struct Vec3 vector)
{
	struct Matrix vectorMatrix = Matrix_create(1, 3);
	*Matrix_element(&vectorMatrix, 0, 0) = vector.x;
	*Matrix_element(&vectorMatrix, 0, 1) = vector.y;
	*Matrix_element(&vectorMatrix, 0, 2) = vector.z;
	return vectorMatrix;
}

/*
	vector2toMatrix2 : Converts a 2D vector to its corresponding 1*2 matrix

*/

struct Matrix vector4toMatrix(struct Vec4 vector)
{
	struct Matrix vectorMatrix = Matrix_create(1, 4);
	*Matrix_element(&vectorMatrix, 0, 0) = vector.x;
	*Matrix_element(&vectorMatrix, 0, 1) = vector.y;
	*Matrix_element(&vectorMatrix, 0, 2) = vector.z;
	*Matrix_element(&vectorMatrix, 0, 3) = vector.w;
	return vectorMatrix;
}



typedef struct Matrix Matrix;

#endif