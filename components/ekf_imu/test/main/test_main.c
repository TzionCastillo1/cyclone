#include "test_mat_utils.c"
#include "test_ekf.c"

int app_main(int argc, char **argv)
{
	UNITY_BEGIN();
	RUN_TEST(test_eye_provides_identity);
	RUN_TEST(test_transpose_works_struct);
	RUN_TEST(test_mat_utils_c_mult_scalar);
	RUN_TEST(test_mat_utils_inverse);
	RUN_TEST(test_mat_utils_skew_quat);
	RUN_TEST(test_mat_utils_quat_norm);
	RUN_TEST(test_get_F);
	RUN_TEST(test_get_H);
	RUN_TEST(test_get_h);
	RUN_TEST(test_mat_utils_copy_block);
	RUN_TEST(test_predict);
	RUN_TEST(test_update);
	int failures = UNITY_END();
	return failures;
}
