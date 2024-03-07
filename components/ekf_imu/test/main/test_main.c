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
	int failures = UNITY_END();
	return failures;
}
