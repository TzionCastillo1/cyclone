#include <stdint.h>
#include "unity.h"
#include "mat_utils.h"

void test_eye_provides_identity()
{
	float eye[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	float test_eye[9];
	mat_utils_eye(test_eye, 3);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(eye, test_eye, 9);
}

void test_transpose_works()
{
	float A[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	float A_transpose_truth[9] = {0, 3, 6, 1, 4, 7, 2, 5, 8};
	float A_transpose_test[9];

	mat_utils_transpose(A, A_transpose_test, 3, 3);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_transpose_truth, A_transpose_test, 9);	
}

void test_mat_utils_c_mult_scalar()
{
	float A[4] = {1, 1, 1, 1};
	float A_mult_truth[4] = {4, 4, 4, 4};
	
	mat_utils_c_mult_scalar(A, 4, 2, 2);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_mult_truth, A, 4);
	
}

void test_mat_utils_c_add()
{
	float A[4] = {0, 0, 0, 0};
	float B[4] = {1, 2, 3, 4};

	mat_utils_c_add(A, B, 2, 2);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A, B, 4);
}

void test_mat_utils_c_sub()
{
	float A[4] = {1, 2, 3, 4};
	float B[4] = {1, 2, 3, 4};
	float A_sub_truth[4] = {0, 0, 0, 0};

	mat_utils_c_sub(A, B, 2, 2);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_sub_truth, A, 4);
}

void test_mat_utils_inverse()
{
	float A[9] = {5.0, 7.0, 9.0, 4.0, 3.0, 8.0, 7.0, 5.0, 6.0};
	float A_inv_truth[9] = {-0.209, 0.0286, 0.276, 
							0.305, -0.314, -0.0381,
							-0.01, 0.229, -0.124};
	float A_inv[9];
	
	mat_utils_inverse_3x3(A, A_inv);

	for (int item = 0; item < 9; item++)
		TEST_ASSERT_FLOAT_WITHIN(.01, A_inv_truth[item], A_inv[item]);
}

void test_mat_utils_skew_quat()
{
	float state[7] = {1, 0, 0, 0, .1, .1, .1};
	float skew_quat_truth[12] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
	float test_skew_quat[12];

	mat_utils_skew_quat(state, test_skew_quat);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(skew_quat_truth, test_skew_quat, 12);
}

void test_mat_utils_quat_norm()
{
	float state[7] = {1, 1, 1, 1, 0, 0, 0};
	float state_norm_truth[4] = {.5, .5, .5, .5};

	mat_utils_quat_norm(state);

	TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_norm_truth, state, 4);
}

/*
int app_main(int argc, char **argv)
{
	UNITY_BEGIN();
	RUN_TEST(test_eye_provides_identity);
	RUN_TEST(test_transpose_works);
	RUN_TEST(test_mat_utils_c_mult_scalar);
	RUN_TEST(test_mat_utils_c_add);
	RUN_TEST(test_mat_utils_c_sub);
	RUN_TEST(test_mat_utils_inverse);
	RUN_TEST(test_mat_utils_skew_quat);
	RUN_TEST(test_mat_utils_quat_norm);
	int failures = UNITY_END();
	return failures;
}
*/
