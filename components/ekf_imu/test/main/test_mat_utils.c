#include <stdint.h>
#include "unity.h"
#include "mat_utils.h"

void test_eye_provides_identity()
{
	float eye[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	float test_eye[9];
	matrixf_t A = {.mat = test_eye, .HEIGHT = 3, .WIDTH = 3};
	mat_utils_eye(&A);
	TEST_ASSERT_EQUAL_FLOAT_ARRAY(eye, A.mat, 9);
}

void test_transpose_works_struct()
{
	float A_mat[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	matrixf_t A = {
		.mat = A_mat,
		.HEIGHT = 3,
		.WIDTH = 3};

	float A_transpose_truth_mat [9] = {0, 3, 6, 1, 4, 7, 2, 5, 8};

	float A_transpose_test[9];
	matrixf_t A_transpose = {
		.mat = A_transpose_test,
		.HEIGHT = 3,
		.WIDTH = 3};


	mat_utils_transpose(&A, &A_transpose);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_transpose_truth_mat, A_transpose.mat, 9);	
}


void test_mat_utils_c_mult_scalar()
{
	float A_mat[4] = {1, 1, 1, 1};
	matrixf_t A = {
		.mat = A_mat,
		.HEIGHT = 2,
		.WIDTH = 2};

	float A_mult_truth[4] = {4, 4, 4, 4};
	
	mat_utils_c_mult_scalar(&A, 4);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_mult_truth, A.mat, 4);
	
}


void test_mat_utils_inverse()
{
	float A_mat[9] = {5.0, 7.0, 9.0, 4.0, 3.0, 8.0, 7.0, 5.0, 6.0};
	float A_inv_truth_mat[9] = {-0.209, 0.0286, 0.276, 
							0.305, -0.314, -0.0381,
							-0.01, 0.229, -0.124};
	float A_inv_mat[9];

	matrixf_t A = {.mat = A_mat, .HEIGHT = 3, .WIDTH = 3};
	matrixf_t A_inv = {.mat = A_inv_mat, .HEIGHT = 3, .WIDTH = 3};
	
	mat_utils_inverse_3x3(&A, &A_inv);

	for (int item = 0; item < 9; item++)
		TEST_ASSERT_FLOAT_WITHIN(.01, A_inv_truth_mat[item], A_inv.mat[item]);
}

void test_mat_utils_skew_quat()
{
	float state[7] = {1, 0, 0, 0, .1, .1, .1};
	float skew_quat_truth[12] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
	float test_skew_quat[12];
	matrixf_t Skew_q = {.mat = test_skew_quat, .HEIGHT = 4, .WIDTH = 3};
	mat_utils_skew_quat(state, &Skew_q);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(skew_quat_truth, Skew_q.mat, 12);
}

void test_mat_utils_quat_norm()
{
	float state[7] = {1, 1, 1, 1, 0, 0, 0};
	float state_norm_truth[4] = {.5, .5, .5, .5};

	mat_utils_quat_norm(state);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_norm_truth, state, 4);
}



