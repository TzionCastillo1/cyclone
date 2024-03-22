#include "ekf_imu.h"
#include <stdint.h>
#include "unity.h"
#include <stdio.h>
#include "array_utils.h"

#define Q_INITIAL_DIAG (.008 * DEGTORAD)
#define R_INITIAL_DIAG (.125 * 9.8)
#define P_INITIAL_DIAG (0)

void test_get_F()
{

	float state[7] = {1,0,0,0,0,0,0};
	float xTest[7];
	ekf_imu_filter_t ekf_imu_filter={.x={7,1,xTest}};
	array_utils_memcpy(state, ekf_imu_filter.x.mat, 7);
	float Fmat[49];
	
	matrixf_t F = {.mat= Fmat, .HEIGHT = 7, .WIDTH = 7};

	float gyro_rates[3] = {0, 0, 1.572604f};
	const float dt = 1.0f/20.0f;
	ekf_imu_get_F(&ekf_imu_filter, &F, gyro_rates, dt);

	float F_truth[49] = {1, 0, 0, -0.0393, 0, 0, 0,
		  				0, 1, 0.0393, 0, 1, 0, 0,
						0, -0.0393, 1, 0, 0, 1, 0,
						0.0393, 0, 0, 1, 0, 0, 1,
						0, 0, 0, 0, 1, 0, 0, 
						0, 0, 0, 0, 0, 1, 0,
						0, 0, 0, 0, 0, 0, 1};
	for (int item = 0; item < 49; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, F_truth[item], F.mat[item]);
}

void test_get_H()
{
	float state[7] = {1,0,0,0.0393,0,0,0};
	float xTest[7];
	ekf_imu_filter_t ekf_imu_filter={.x={7,1,xTest}};
	array_utils_memcpy(state, ekf_imu_filter.x.mat, 7);

	float Hmat[21];
	matrixf_t H = {.mat = Hmat, .HEIGHT = 3, .WIDTH = 7};

	ekf_imu_get_H(&ekf_imu_filter, &H);

	float H_truth[21] = {0, 0.0786, 2.0, 0, 0, 0, 0,
						0, -2.0, 0.0786, 0, 0, 0, 0,
						2.0, 0, 0, 0.0786, 0, 0, 0};

	for (int item = 0; item < 21; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, H_truth[item], H.mat[item]);
}

void test_get_h()
{
	float state[7] = {1,0,0,0.0393,0,0,0};
	float xTest[7];
	ekf_imu_filter_t ekf_imu_filter={.x={7,1,xTest}};
	array_utils_memcpy(state, ekf_imu_filter.x.mat, 7);

	float hmat[3];
	matrixf_t h = {.mat = hmat, .HEIGHT = 3, .WIDTH = 7};

	ekf_imu_get_h(&ekf_imu_filter, &h);

	float h_truth[3] = {0, 0, 9.8151};

	for (int item = 0; item < 3; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, h_truth[item], h.mat[item]);
}
	
void test_predict()
{
	ekf_imu_config_t ekf_imu_config;
	ekf_imu_set_default_params(&ekf_imu_config, R_INITIAL_DIAG, Q_INITIAL_DIAG, P_INITIAL_DIAG);

	float x_init_mat[7] = {1.0, 0, 0, 0, 0, 0, 0};
	const matrixf_t x_init = {.mat = x_init_mat, .HEIGHT = 7, .WIDTH = 1};

	mat_utils_copy(&x_init, &ekf_imu_config.x_init);
	
	ekf_imu_filter_t ekf_imu_filter;
	ekf_imu_init(&ekf_imu_config, &ekf_imu_filter);

	float dt = 1.0/20.f;
	float gyro_rates[3] = {0, 0, 1.572604f};
	ekf_imu_predict_state(&ekf_imu_filter, gyro_rates, dt);

	float x_pred_truth[7] = {1.0, 0, 0, 0.0393, 0, 0, 0};

	for (int item = 0; item < 7; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, x_pred_truth[item], ekf_imu_filter.x.mat[item]);

}

void test_update()
{
	
	ekf_imu_config_t ekf_imu_config;
	ekf_imu_set_default_params(&ekf_imu_config, R_INITIAL_DIAG, Q_INITIAL_DIAG, P_INITIAL_DIAG);

	float x_init_mat[7] = {1.0, 0, 0, 0.0393, 0, 0, 0};
	const matrixf_t x_init = {.mat = x_init_mat, .HEIGHT = 7, .WIDTH = 1};

	mat_utils_copy(&x_init, &ekf_imu_config.x_init);
	
	ekf_imu_filter_t ekf_imu_filter;
	ekf_imu_init(&ekf_imu_config, &ekf_imu_filter);

	float P_temp_mat[49] = {.1396, 0, 0, 0, 0, 0, 0,
						0, 0.1396, 0, 0, 0, 0, 0,
						0, 0, 0.1396, 0, 0, 0, 0,
						0, 0, 0, 0.1396, 0, 0, 0,
						0, 0, 0, 0, 0.1396, 0, 0,
						0, 0, 0, 0, 0, 0.1396, 0,
						0, 0, 0, 0, 0, 0, 0.1396};
	matrixf_t P_init = {.mat = P_temp_mat, .HEIGHT = 7, .WIDTH = 7};
	mat_utils_copy(&P_init, &ekf_imu_filter.P);
	mat_utils_c_mult_scalar(&ekf_imu_filter.P, .001f);

	float acc_measurements[3] = {0.0003, 0.0009, 9.8079};
	ekf_imu_update_state(&ekf_imu_filter, acc_measurements);

	float x_truth_mat[7] = {.9992, -0, 0, 0.0393, 0, 0, 0};
	
	for(int item = 0; item < 7; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, x_truth_mat[item], ekf_imu_filter.x.mat[item]); 

	float P_truth_mat[49] = {.1396, 0, 0, -0, 0, 0, 0,
							0, .1396, 0, 0, 0, 0, 0,
							0, -0, .1396, 0, 0, 0, 0,
							-0, 0, 0, .1396, 0, 0, 0,
							0, 0, 0, 0, .1396, 0, 0, 
							0, 0, 0, 0, 0,  .1396, 0, 
							0, 0, 0, 0, 0, 0, .1396};
	matrixf_t P_truth = {.mat=P_truth_mat, .HEIGHT = 7, .WIDTH = 7};
	mat_utils_c_mult_scalar(&P_truth, .001f);

	for(int item = 0; item < 49; item++)
		TEST_ASSERT_FLOAT_WITHIN(.0001, P_truth_mat[item], ekf_imu_filter.P.mat[item]);
}


