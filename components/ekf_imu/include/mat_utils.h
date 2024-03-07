#ifndef MAT_UTILS_H
#define MAT_UTILS_H

#include <stdint.h>

typedef struct
{
	const uint8_t HEIGHT;
	const uint8_t WIDTH;
	float* mat;
} matrixf_t;

/**
 * @brief function to copy the contents from one matrix into another.
 *
 * @param A: matrix to be copied
 * @param B: destination of copied data
 */
void mat_utils_copy(const matrixf_t* A, matrixf_t* B); 

void mat_utils_transpose(const matrixf_t* A, matrixf_t* A_transpose);

/* NOTE: Create an identity matrix of size: A_size */
void mat_utils_eye(matrixf_t* A);

void mat_utils_zeros(matrixf_t* A);

void mat_utils_c_mult_scalar(matrixf_t* A, const float scalar);

void mat_utils_mult_scalar(const matrixf_t* A, matrixf_t* B, const float Scalar);

void mat_utils_mult(const float* A, const float* B, float C, const uint8_t A_height, const uint8_t A_width, const uint8_t B_width);

void mat_utils_c_add(matrixf_t* A, const matrixf_t* B);

void mat_utils_c_sub(matrixf_t* A, const matrixf_t* B);

/* NOTE: mat_utils_inverse requires the input array to be square */
void mat_utils_inverse_3x3(const matrixf_t* A, matrixf_t* A_inv);

void mat_utils_skew_quat(const float* q, matrixf_t* S_q);

void mat_utils_quat_norm(float* q);

#endif /* mat_utils_h */
