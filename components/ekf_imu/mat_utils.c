#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "mat_utils.h"
#include "array_utils.h"


void mat_utils_transpose(const matrixf_t* A, matrixf_t* A_transpose)
{
	for(int row = 0; row < A->HEIGHT; row++)
	{
		for(int column = 0; column < A->WIDTH; column++)
			A_transpose->mat[((column * A->HEIGHT) + row)] = A->mat[((row * A->WIDTH) + column)];
	}
}

void mat_utils_eye(matrixf_t* A)
{
	//TODO: Write assert function if(A->HEIGHT != A->WIDTH)
	for(int item = 0; item < (A->HEIGHT * A->WIDTH); item++)
	{
		A->mat[item] = 0;
	}
	for(int row = 0; row < A->HEIGHT; row++)
	{
		A->mat[(A->HEIGHT + 1) * row] = 1.0f;
	}
}

void mat_utils_mult_scalar(const matrixf_t* A, matrixf_t* B, const float scalar)
{
	for(int item = 0; item < (A->HEIGHT * A->WIDTH); item++)
		B->mat[item] = scalar * A->mat[item];
}

void mat_utils_c_mult_scalar(matrixf_t* A, const float scalar)
{
	for(int item = 0; item < (A->HEIGHT * A->WIDTH); item++)
		A->mat[item] = scalar * (A->mat[item]);
}

void mat_utils_c_add(matrixf_t* A, const matrixf_t* B)
{
	for(int item = 0; item < (A->HEIGHT * A->WIDTH); item++)
		A->mat[item] = A->mat[item] + B->mat[item];
}

void mat_utils_c_sub(matrixf_t* A, const matrixf_t* B)
{
	for(int item = 0; item < (A->HEIGHT * A->WIDTH); item++)
		A->mat[item] = A->mat[item] - B->mat[item];
	
}

void mat_utils_inverse_3x3(const matrixf_t* A, matrixf_t* A_inv)
{
	// NOTE: To begin, we augment the matrix with an Identity matrix
	// NOTE: For simplicity, we represent the augmented matrix as a
	// multidimensional array.
	
	float A_aug[3][6] = {{A->mat[0], A->mat[1], A->mat[2], 1, 0, 0},
						{A->mat[3], A->mat[4], A->mat[5], 0, 1, 0},
						{A->mat[6], A->mat[7], A->mat[8], 0, 0, 1}};
	// NOTE: After we create the augmented matrix, we will ensure that
	// the first column is in ascending order by swapping the rows around
	// as necessary.
	// NOTE: The matrix rows cannot be swapped by swapping pointers.
	
	for (uint8_t row = 0; row < 2; row++)
	{
		if (A_aug[row][0] < A_aug[row + 1][0])
		{
			float temp_row[6];
			for(int item = 0; item < 6; item++)
			{
				temp_row[item] = A_aug[row][item];
				A_aug[row][item] = A_aug[row+1][item];
				A_aug[row+1][item] = temp_row[item];
			}
			//If the first element was less than the previous element in the column.
			//recheck the prior element against this one.
			row-=2;
		}
	}

	//Loop for performing the Gauss-Jordan elemination.
	//We start by making the left matrix upper-triangular.
	for(int pivot = 0; pivot < 2; pivot++)
	{
		for(int row = pivot + 1; row < 3; row++)
		{
			float temp_row[6];
			array_utils_mult_scalar_f(A_aug[pivot], temp_row, (A_aug[row][pivot]/A_aug[pivot][pivot]),  6);
			array_utils_sub_f(A_aug[row], temp_row, A_aug[row], 6);
		}
	}

	//Now the left matrix is converted to be diagonal.
	for(int pivot = 2; pivot > 0; pivot--)
	{
		for(int row = pivot - 1; row >= 0; row--)
		{
			float temp_row[6];
			array_utils_mult_scalar_f(A_aug[pivot], temp_row, (A_aug[row][pivot]/A_aug[pivot][pivot]), 6);
			array_utils_sub_f(A_aug[row], temp_row, A_aug[row], 6);
		}
	}
	
	//Finally, both matrixes are scaled such that the left matrix is the identity matrix.
	for(int row = 0; row < 3; row++)
		array_utils_mult_scalar_f(A_aug[row], A_aug[row], (1/A_aug[row][row]), 6);

	//Now we copy the contents of the left side of the augmented matrix to our output.
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			A_inv->mat[3*row + col] = A_aug[row][col+3];
		}
	}

}


void mat_utils_skew_quat(const float* q, matrixf_t* S_q)
{
	float S_q_temp[12] = {-q[1], -q[2], -q[3],
						q[0], -q[3], q[2],
						q[3], q[0], -q[1],
						-q[2], q[1], q[0]};

	for(int item = 0; item < 12; item++)
		S_q->mat[item] = S_q_temp[item];
}

void mat_utils_quat_norm(float* q)
{
	float squared_sum = 0;
	for(int item = 0; item < 4; item++)
		squared_sum+= (q[item] * q[item]);

	float magnitude = sqrtf(squared_sum);
	
	for(int item = 0; item < 4; item++)
		q[item] = q[item] / magnitude;
}
