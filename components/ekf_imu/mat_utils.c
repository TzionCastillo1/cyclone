#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "mat_utils.h"

void mat_utils_eye(float* A, const uint8_t A_size)
{
	for(int item = 0; item < (A_size * A_size); item++)
	{
		A[item] = 0;
	}
	for(int row = 0; row < A_size; row++)
	{
		A[(A_size + 1) * row] = 1.0f;
	}
}

void mat_utils_transpose(const float* A, float* A_transpose, const uint8_t A_height, const uint8_t A_width)
{
	for(int row = 0; row < A_height; row++)
	{
		for(int column = 0; column < A_width; column++)
			A_transpose[((column* A_height) + row)] = A[((row * A_width) + column)];
	}
}

void mat_utils_mult_scalar(const float* A, const float scalar, float* B, const uint8_t A_height, const uint8_t A_width)
{
	for(int item = 0; item < (A_height * A_width); item++)
		B[item] = scalar * A[item];
}

void mat_utils_c_mult_scalar(float* A, const float scalar, const uint8_t A_height, const uint8_t A_width)
{
	for(int item = 0; item < (A_height * A_width); item++)
		A[item] = scalar * A[item];
}

void mat_utils_c_add(float* A, const float* B, const uint8_t height, const uint8_t width)
{
	for(int item = 0; item < (height * width); item++)
		A[item] = A[item] + B[item];
}

void mat_utils_c_sub(float* A, const float* B, const uint8_t height, const uint8_t width)
{
	for(int item = 0; item < (height * width); item++)
		A[item] = A[item] - B[item];
	
}

void mat_utils_inverse_3x3(const float* A, float* A_inv)
{
	// NOTE: To begin, we augment the matrix with an Identity matrix
	// NOTE: For simplicity, we represent the augmented matrix as a
	// multidimensional array.
	
	float A_aug[3][6] = {{A[0], A[1], A[2], 1, 0, 0},
						{A[3], A[4], A[5], 0, 1, 0},
						{A[6], A[7], A[8], 0, 0, 1}};
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
			mat_utils_mult_scalar(A_aug[pivot], (A_aug[row][pivot]/A_aug[pivot][pivot]), temp_row, 1, 6);
			mat_utils_c_sub(A_aug[row], temp_row, 1, 6);
		}
	}

	//Now the left matrix is converted to be diagonal.
	for(int pivot = 2; pivot > 0; pivot--)
	{
		for(int row = pivot - 1; row >= 0; row--)
		{
			float temp_row[6];
			mat_utils_mult_scalar(A_aug[pivot], (A_aug[row][pivot]/A_aug[pivot][pivot]), temp_row, 1, 6);
			mat_utils_c_sub(A_aug[row], temp_row, 1, 6);
		}
	}
	
	//Finally, both matrixes are scaled such that the left matrix is the identity matrix.
	for(int row = 0; row < 3; row++)
		mat_utils_c_mult_scalar(A_aug[row], (1/A_aug[row][row]), 6, 1);

	//Now we copy the contents of the left side of the augmented matrix to our output.
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			A_inv[3*row + col] = A_aug[row][col+3];
		}
	}

}


void mat_utils_skew_quat(const float* q, float* S_q)
{
	float S_q_temp[12] = {-q[1], -q[2], -q[3],
						q[0], -q[3], q[2],
						q[3], q[0], -q[1],
						-q[2], q[1], q[0]};

	for(int item = 0; item < 12; item++)
		S_q[item] = S_q_temp[item];
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
